// Fill out your copyright notice in the Description page of Project Settings.
#include "HeathComponent.h"

UHeathComponent::UHeathComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UHeathComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;

	AActor* MyOwner = GetOwner();
	if (MyOwner != nullptr)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHeathComponent::HandleTakeAnyDamage);
	}
}

void UHeathComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("Getting Damage"));

	if (Damage <= 0.f || bIsDead) return;
	if (DamageCauser == DamagedActor) return;

	UHeathComponent* OtherHealthComp = Cast<UHeathComponent>(DamageCauser->FindComponentByClass(UHeathComponent::StaticClass()));
	if (OtherHealthComp)
	{
		if (OtherHealthComp->TeamNum == TeamNum) return;
	}

	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	bIsDead = Health <= 0.f;

	OnHealthChanged.Broadcast(Health, Damage, DamageCauser);

	if (bIsDead)
	{
		OnDead.Broadcast(DamageCauser);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
}

void UHeathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UHeathComponent::GetHealthPercent() const
{
	return Health / DefaultHealth;
}

void UHeathComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.f || Health <= 0.f) return;
	Health = FMath::Clamp(Health + HealAmount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health, -HealAmount, nullptr);
}