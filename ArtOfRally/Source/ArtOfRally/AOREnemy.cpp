#include "AOREnemy.h"
#include "HeathComponent.h"

AAOREnemy::AAOREnemy()
{
 	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UHeathComponent>(FName("HealthComp"));
}

void AAOREnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AAOREnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAOREnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AAOREnemy::GetHealth()
{
	return HealthComp != nullptr ? HealthComp->Health : 0.f;
}

bool AAOREnemy::GetIsDead()
{
	return HealthComp != nullptr ? HealthComp->bIsDead : false;
}
