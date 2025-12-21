// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Sound/SoundCue.h"
#include "Projectile.h"
#include "KombatComponent.h"
#include "AOREnemy.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeaponBase::Shoot()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(Handle_FireMode_AUTO))
	GetWorld()->GetTimerManager().SetTimer(Handle_FireMode_AUTO, this, &AWeaponBase::ShootLogic, ShotRate, true, 0.01f);
}

void AWeaponBase::StopShot()
{
	GetWorld()->GetTimerManager().ClearTimer(Handle_FireMode_AUTO);
}

void AWeaponBase::ShootLogic()
{
	if (!FindRifleTarget()) return;

	FHitResult HitResult;
	
	if (Fire(GetWorld(),HitResult))
	{
		ImpactEvent(HitResult.Location);
		ApplyDamage(HitResult);
	}

	if (BulletSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BulletSound, HitResult.Location);
	}
	
}

void AWeaponBase::ApplyDamage(FHitResult HitResult)
{
	if (HitResult.bBlockingHit)
	{
		//UGameplayStatics::ApplyDamage(HitResult.GetActor(), DamagePerShot, GetOwner()->GetInstigatorController(), GetOwner(), NULL);
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), DamagePerShot, GetWorld()->GetFirstPlayerController(), GetWorld()->GetFirstPlayerController()->GetPawn(), NULL);
		//else UE_LOG(LogTemp, Error, TEXT("Owner Is nullptr For %s, Try To Call AActor::SetOwner() Before Calling Fire()"), *GetName());
	}
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	UActorComponent* AC_Shoot = FindComponentByTag(USceneComponent::StaticClass(), FName("Shoot"));
	if (AC_Shoot) ShootComp = Cast<USceneComponent>(AC_Shoot);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeaponBase::Fire(UWorld* WorldPtr, FHitResult& HitResult)
{
	if (!ShootComp)
	{
		UE_LOG(LogTemp, Error, TEXT("ShootComp Is Null"));
		return false;
	}

	if (WeaponType != EWeaponType::EWT_MachineGun) return false;

	FVector StartLocation = ShootComp->GetComponentLocation();
	FVector EndLocation = StartLocation + ShootComp->GetComponentRotation().Vector() * WeaponRange;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = true;;
	bool bSuccess = WorldPtr->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, QueryParams);

	//FColor TraceColor = bSuccess ? FColor::Green : FColor::Red;
	//DrawDebugLine(
	//	WorldPtr,
	//	StartLocation,
	//	bSuccess ? HitResult.Location : EndLocation,
	//	TraceColor,
	//	false,   // persistent lines?
	//	2.0f,    // lifetime
	//	0,       // depth priority
	//	2.0f     // thickness
	//);
	
	if (bSuccess)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Hit On : %s"), *HitResult.GetActor()->GetName()));
	}
	else HitResult.Location = EndLocation;

	FireEvent();

	return bSuccess;
}

void AWeaponBase::LaunchProjectile()
{
	if (WeaponType != EWeaponType::EWT_Projetile) return;

	if (FindTarget())
	{
		if (GetSuggestedProjectileVelocity(CurrentLaunchVelocity))
		{
			UpdatingSwitch(true);
			FRotator LaunchRotation = CurrentLaunchVelocity.Rotation();

			LaunchRotation.Normalize();
			RotationYawOffset = LaunchRotation.Yaw;
			RotationFix(RotationYawOffset);

			GetWorld()->GetTimerManager().SetTimer(Handle_LaunchDelayProjectile, this, &AWeaponBase::FinalLaunchProjectile, LaunchDelay, false);
		}
		else
		{
			UpdatingSwitch(false);
		}
	}
	else
	{
		UpdatingSwitch(false);
	}
}

bool AWeaponBase::GetSuggestedProjectileVelocity(FVector& OutVelocity)
{
	if (!CurrentTarget) return false;

	FVector StartLocation = GetActorLocation();
	FVector TargetLocation = CurrentTarget->GetActorLocation();

	bool bHasSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutVelocity,          // OUT: suggested velocity
		StartLocation,           // Start point
		TargetLocation,          // End point
		LaunchForce,                // Launch speed (units/sec)
		false,                   // High arc (true) or low arc (false)
		0.0f,                    // Collision radius
		0.0f,                    // Override gravity (0 = use world gravity)
		ESuggestProjVelocityTraceOption::DoNotTrace // tracing option
	);


	return bHasSolution;
}

bool AWeaponBase::FindTarget()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAOREnemy::StaticClass(), OutActors);

	float Distance = FLT_MAX;
	AAOREnemy* Target = nullptr;
	bool bEResult = false;

	for (AActor* Item : OutActors)
	{
		if (Item != nullptr)
		{
			AAOREnemy* Enemy = Cast<AAOREnemy>(Item);
			if (Enemy != nullptr)
			{
				if (!Enemy->GetIsDead())
				{
					double CurrentDistance = (Enemy->GetActorLocation() - GetActorLocation()).Size();
					if (CurrentDistance < Distance)
					{
						Distance = CurrentDistance;
						Target = Enemy;
					}
				}
			}
		}
	}

	CurrentTarget = Target;
	return CurrentTarget != nullptr;
}

bool AWeaponBase::FindRifleTarget()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAOREnemy::StaticClass(), OutActors);

	bool bResult = false;

	for (AActor* Item : OutActors)
	{
		if (Item != nullptr)
		{
			AAOREnemy* Enemy = Cast<AAOREnemy>(Item);
			if (Enemy != nullptr)
			{
				if (!Enemy->GetIsDead())
				{

					FVector ToEnemy = (Enemy->GetActorLocation() - ShootComp->GetComponentLocation()).GetSafeNormal();
					FVector Forward = ShootComp->GetForwardVector();


					float DotProduct = FVector::DotProduct(Forward, ToEnemy);
					float AngleRadians = FMath::Acos(DotProduct);
					float Angle = UKismetMathLibrary::RadiansToDegrees(AngleRadians);

					//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Angle : %f"), Angle));

					if (FMath::Abs(Angle) < MinimumAngleToShoot)
					{
						//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("{}Angle : %f"), FMath::Abs(Angle)));
						return true;
					}
					
				}
			}
		}
	}

	return bResult;
}

void AWeaponBase::FinalLaunchProjectile()
{
	FVector StartLocation = ShootComp->GetComponentLocation();
	FRotator Rot = ShootComp->GetComponentRotation();
	FVector Direction = Rot.Vector();
	AProjectile* ProjectileBullet = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, StartLocation, Rot);
	if (ProjectileBullet != nullptr)
	{
		ProjectileBullet->SetDamage(ProjectileDamage);
		ProjectileBullet->SetOwner(this->GetOwner());
		if (KombatComp != nullptr)
		{
			ProjectileBullet->SetTarget(CurrentTarget);
			ProjectileBullet->SetProjectileVelocity(CurrentLaunchVelocity);
		}
		FireEvent();
	}
}