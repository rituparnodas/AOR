// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AOREnemy.generated.h"

class UHeathComponent;

UCLASS()
class ARTOFRALLY_API AAOREnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAOREnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHeathComponent* HealthComp = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	bool GetIsDead();


	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void StartShoot();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void StopShoot();
};
