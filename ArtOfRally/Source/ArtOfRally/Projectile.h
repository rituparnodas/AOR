// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class AAOREnemy;

UCLASS()
class ARTOFRALLY_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjMoveComp = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode();

	UFUNCTION()
	void OnProjectileCollide(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetTarget(AAOREnemy* InTarget);

	void SetDamage(float InDamage);

	void SetProjectileVelocity(const FVector InVelocity);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DNA")
	float FollowSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DNA")
	float RotationSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DNA")
	float MinTargetReachedDistanceToExplode = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DNA")
	float LinearSpeed = 21.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DNA")
	float MaxLinearRange = 20000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DNA")
	float FollowStartDelay = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DNA")
	float DamageRadius = 500.f;

private:
	float DamageAmount = 10.f;
	AActor* TargetActor = nullptr;
	FVector TargetOffset;
	FVector InitialLocation;
	bool bUTurnToTarget = false;

	bool bDestroyed = false;

private:

	FVector LaunchVelocity;

};
