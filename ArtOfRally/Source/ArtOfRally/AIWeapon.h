// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIWeapon.generated.h"

UCLASS()
class ARTOFRALLY_API AAIWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIWeapon();

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintCallable)
	void StopShot();

	void ShootLogic();
	void ApplyDamage(FHitResult HitResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
