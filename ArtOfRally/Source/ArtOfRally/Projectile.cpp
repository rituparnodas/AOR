// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AOREnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (OtherActor != GetOwner())
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		
		//UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetWorld()->GetFirstPlayerController(), GetWorld()->GetFirstPlayerController()->GetPawn(), NULL);

		UGameplayStatics::ApplyRadialDamage(
			this,
			DamageAmount,                // base damage
			Hit.ImpactPoint,             // center of explosion
			DamageRadius,                // radius
			UDamageType::StaticClass(),  // damage type
			IgnoreActors,           // actors to ignore
			this,                        // damage causer
			GetInstigatorController(),   // instigator
			true                         // do full damage
		);

		// Optional: debug sphere
		//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, DamageRadius, 8, FColor::Red, false, 10.0f);

		ApplyExplosionForce();

		Explode();
	}
}

void AProjectile::ApplyExplosionForce()
{
	TArray<AActor*> OverlappingActors;

	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),          // center
		DamageRadius,                     // radius
		{ EObjectTypeQuery::ObjectTypeQuery3, EObjectTypeQuery::ObjectTypeQuery4 },
		// ObjectTypeQuery3 = Pawn, ObjectTypeQuery4 = PhysicsBody (depends on project settings)
		nullptr,                    // class filter
		{this},          // actors to ignore
		OverlappingActors           // OUT: actors found
	);

	for (AActor* Actor : OverlappingActors)
	{
		if (AAOREnemy* HitEnemy = Cast<AAOREnemy>(Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Applying Character Force: %s"), *(HitEnemy->GetName()));
			FVector Dir = (HitEnemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			Dir = UKismetMathLibrary::Quat_RotateVector(FQuat::MakeFromRotator(FRotator(45.f, 0.f,0.f)), Dir);
			HitEnemy->LaunchCharacter(Dir * LaunchForcePawn, false, false);
		}
		else if (UPrimitiveComponent* PrimComp = Actor->FindComponentByClass<UPrimitiveComponent>())
		{
			if (PrimComp->IsSimulatingPhysics())
			{
				UE_LOG(LogTemp, Warning, TEXT("Applying Force: %s"), *(Actor->GetName()));

				FVector Direction = (PrimComp->GetComponentLocation() - GetActorLocation()).GetSafeNormal();
				Direction = UKismetMathLibrary::Quat_RotateVector(FQuat::MakeFromRotator(FRotator(45.f, 0.f, 0.f)), Direction);
				PrimComp->AddImpulse(Direction * ExplosionForce, NAME_None ,false);
			}
		}
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