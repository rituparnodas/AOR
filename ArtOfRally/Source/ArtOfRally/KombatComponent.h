// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KombatComponent.generated.h"

class AWeaponBase;
class USceneComponent;
class AAOREnemy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARTOFRALLY_API UKombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKombatComponent();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponBase> MiniGunClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponBase> LauncherClass;

	void SpawnMiniGun();

	void SpawnFrontLauncher();

	UFUNCTION(BlueprintCallable)
	void SpawnBackLauncher();

public:

	UFUNCTION(BlueprintCallable)
	void IncreaseAllWeaponDamage(float IncreaseRate = 1.2f);

	UFUNCTION(BlueprintCallable)
	void ResetAllWeaponDamage();

	//UPROPERTY(BlueprintReadWrite)
	//AAOREnemy* CurrentProjectileTarget = nullptr;
	//
	//UFUNCTION(BlueprintCallable)
	//bool FindTarget();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
	UPROPERTY(BlueprintReadOnly)
	AWeaponBase* MiniGunLeft = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AWeaponBase* MiniGunRight = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AWeaponBase* FrontLeftLauncher = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AWeaponBase* FrontRightLauncher = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AWeaponBase* BackLeftLauncher = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AWeaponBase* BackRightLauncher = nullptr;

private:
	USceneComponent* LeftMiniSpawn = nullptr;
	USceneComponent* RightMiniSpawn = nullptr;

	USceneComponent* FrontLeftLauncherSpawn = nullptr;
	USceneComponent* FrontRightLauncherSpawn = nullptr;

	USceneComponent* BackLeftLauncherSpawn = nullptr;
	USceneComponent* BackRightLauncherSpawn = nullptr;
};
