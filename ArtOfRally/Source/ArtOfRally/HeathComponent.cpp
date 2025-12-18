// Fill out your copyright notice in the Description page of Project Settings.
#include "HeathComponent.h"

UHeathComponent::UHeathComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Health = DefaultHealth;
}

void UHeathComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner != nullptr)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHeathComponent::HandleTakeAnyDamage);
	}
}

void UHeathComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Getting Damage"));

	if (Damage <= 0.f || bIsDead) return;
	if (DamageCauser == DamagedActor) return;

	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	bIsDead = Health <= 0.f;

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
}

void UHeathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

