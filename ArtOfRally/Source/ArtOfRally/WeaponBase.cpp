// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Sound/SoundCue.h"
#include "Projectile.h"
#include "KombatComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	FHitResult HitResult;
	
	if (Fire(GetWorld(),HitResult))
	{
		ApplyDamage(HitResult);
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BulletSound, HitResult.Location);
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
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;;
	bool bSuccess = WorldPtr->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	if (bSuccess)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Hit On : %s"), *HitResult.GetActor()->GetName()));
	}
	else HitResult.Location = EndLocation;
	return bSuccess;
	
	return true;
}

void AWeaponBase::LaunchProjectile()
{
	if (WeaponType != EWeaponType::EWT_Projetile) return;

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
			ProjectileBullet->SetTarget(KombatComp->CurrentProjectileTarget);
		}
	}
}