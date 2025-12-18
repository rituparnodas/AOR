// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AOREnemy.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(FName("CollisionComponent"));
	RootComponent = CollisionComp;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLocation = GetActorLocation();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileCollide);

	if (!GetWorld()->GetTimerManager().IsTimerActive(Handle_FollowStart))
		GetWorld()->GetTimerManager().SetTimer(Handle_FollowStart, this, &AProjectile::FollowStart, FollowStartDelay, false);
}

void AProjectile::Explode()
{
	if (!bDestroyed)
	{
		bDestroyed = true;
		
		Destroy();
		//SetLifeSpan(1.f);

		
	}
}

void AProjectile::FollowToTarget(float Dt)
{
	if (TargetActor != nullptr)
	{
		//FVector LocalToWorld = UKismetMathLibrary::TransformLocation(TargetActor->GetActorTransform(), TargetOffset);
		FVector LocalToWorld = TargetActor->GetActorLocation();

		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), LocalToWorld, Dt, FollowSpeed);
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LocalToWorld), Dt, RotationSpeed);

		FHitResult HitResult;
		SetActorLocation(NewLocation, true, &HitResult);
		SetActorRotation(NewRotation);
		//if (HitResult.bBlockingHit || (GetActorLocation() - LocalToWorld).Size() <= MinTargetReachedDistanceToExplode)
		//{
		//	Explode();
		//}
	}
}

void AProjectile::GoWithTheFlow(float Dt)
{
	FVector Location = GetActorLocation();
	Location += GetActorRotation().Vector() * LinearSpeed * Dt;

	FHitResult HitResult;
	SetActorLocation(Location, true, &HitResult);

	//if (HitResult.bBlockingHit || (InitialLocation - GetActorLocation()).Size() >= MaxLinearRange)
	//{
	//	Explode();
	//}
}

void AProjectile::OnProjectileCollide(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AProjectile* SelfCheck = Cast<AProjectile>(OtherActor);
	if (SelfCheck) return;

	if (OtherActor != GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Applying Damage"));
		UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetWorld()->GetFirstPlayerController(), GetWorld()->GetFirstPlayerController()->GetPawn(), NULL);
		Explode();
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUTurnToTarget) FollowToTarget(DeltaTime);
	else GoWithTheFlow(DeltaTime);
}

void AProjectile::SetTarget(AAOREnemy* InTarget)
{
	TargetActor = InTarget;
	if (TargetActor != nullptr)
	{
		TargetOffset = TargetActor->GetActorLocation();
	}
}

void AProjectile::SetDamage(float InDamage)
{
	DamageAmount = InDamage;
}

void AProjectile::FollowStart()
{
	bUTurnToTarget = true;
}

