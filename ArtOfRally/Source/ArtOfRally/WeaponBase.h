// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AProjectile;
class UKombatComponent;
class AAOREnemy;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Meele,
	EWT_MachineGun,
	EWT_Projetile
};

USTRUCT(BlueprintType)
struct FTraceParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float WeaponRange = 1000.f;

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> IgnoreActors;

	UPROPERTY(BlueprintReadWrite)
	bool bTraceComplex = true;

	UPROPERTY(BlueprintReadWrite)
	bool bReturnPhysicsMaterial = false;
};

USTRUCT(BlueprintType)
struct FFXParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UParticleSystem* MuzzleEffect = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UParticleSystem* TracerEffect = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FName TracerTargetName = FName("Tracer");

	UPROPERTY(BlueprintReadWrite)
	UParticleSystem* HitEffect = nullptr;

	UPROPERTY(BlueprintReadWrite)
	USoundCue* MuzzleSound = nullptr;

	UPROPERTY(BlueprintReadWrite)
	USoundCue* HitSound = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* BulletHole = nullptr; // TODO Setup Later
};

USTRUCT(BlueprintType)
struct FProjectileParams
{
	GENERATED_BODY()

	//UPROPERTY(BlueprintReadWrite)
	//TSubclassOf<AProjectileBase> ProjectileClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float LaunchForce = 2000.f;

	UPROPERTY(BlueprintReadWrite)
	float Damage;

	UPROPERTY(BlueprintReadWrite)
	AActor* TargetedActor = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FVector TargetLocation;
};

USTRUCT(BlueprintType)
struct FFireWeaponParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* ShootComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FTraceParams TraceParam;

	UPROPERTY(BlueprintReadWrite)
	FProjectileParams ProjectileParam;

	UPROPERTY(BlueprintReadWrite)
	FFXParams FXParams;
};

UCLASS()
class ARTOFRALLY_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:

	class USceneComponent* ShootComp = nullptr;
	UKombatComponent* KombatComp = nullptr;
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintCallable)
	void StopShot();

	void ShootLogic();
	void ApplyDamage(FHitResult HitResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	EWeaponType WeaponType = EWeaponType::EWT_MachineGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	bool bIsAIWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float DamagePerShot = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float ShotRate = 0.18f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float WeaponRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float MinimumAngleToShoot = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	class USoundCue* BulletSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float ProjectileDamage = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|ProjectileV2")
	float LaunchForce = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|ProjectileV2")
	float LaunchDelay = 0.5f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool Fire(UWorld* WorldPtr, FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile();

	bool FindTarget();

	bool FindRifleTarget();

	bool GetSuggestedProjectileVelocity(FVector& OutVelocity);

	UFUNCTION(BlueprintImplementableEvent)
	void RotationFix(float Yaw);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatingSwitch(bool bShouldUpdate);

	UFUNCTION(BlueprintImplementableEvent)
	void FireEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void ImpactEvent(FVector ImpactLocation);

	void IncreaseDamage(float IncreaseRate = 1.2f);
	void ResetDamage();

public:
	UPROPERTY(BlueprintReadWrite)
	AAOREnemy* CurrentTarget = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float RotationYawOffset = 0.f;

private:
	FTimerHandle Handle_FireMode_AUTO;
	FTimerHandle Handle_LaunchDelayProjectile;

	void FinalLaunchProjectile();

	FVector CurrentLaunchVelocity = FVector::ZeroVector;

	public:
		float DefaultDamagePerShot;
		float DefaultProjectileDamage;
};
