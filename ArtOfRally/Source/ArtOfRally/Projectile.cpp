// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AOREnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(FName("CollisionComponent"));
	RootComponent = CollisionComp;

	ProjMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComponent"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLocation = GetActorLocation();

	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileCollide);
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnProjectileHit);
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

void AProjectile::SetProjectileVelocity(const FVector InVelocity)
{
	if (ProjMoveComp)
	{
		ProjMoveComp->Velocity = InVelocity;
	}
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

void AProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AProjectile* SelfCheck = Cast<AProjectile>(OtherActor);
	if (SelfCheck) return;

	if (OtherActor != GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Applying Damage"));
		//UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetWorld()->GetFirstPlayerController(), GetWorld()->GetFirstPlayerController()->GetPawn(), NULL);

		UGameplayStatics::ApplyRadialDamage(
			this,
			DamageAmount,                // base damage
			Hit.ImpactPoint,             // center of explosion
			DamageRadius,                // radius
			UDamageType::StaticClass(),  // damage type
			TArray<AActor*>(),           // actors to ignore
			this,                        // damage causer
			GetInstigatorController(),   // instigator
			true                         // do full damage
		);

		// Optional: debug sphere
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, DamageRadius, 8, FColor::Red, false, 10.0f);

		Explode();
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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