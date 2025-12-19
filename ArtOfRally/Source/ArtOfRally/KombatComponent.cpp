// Fill out your copyright notice in the Description page of Project Settings.


#include "KombatComponent.h"
#include "WeaponBase.h"
#include "Components/SceneComponent.h"
#include "HeathComponent.h"
#include "AOREnemy.h"
#include "Kismet/GameplayStatics.h"

UKombatComponent::UKombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UKombatComponent::SpawnMiniGun()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	MiniGunLeft = GetWorld()->SpawnActor<AWeaponBase>(MiniGunClass, LeftMiniSpawn->GetComponentTransform(), SpawnParams);
	if (MiniGunLeft)
	{
		MiniGunLeft->AttachToComponent(LeftMiniSpawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		MiniGunLeft->SetOwner(GetOwner());
		MiniGunLeft->KombatComp = this;
	}

	MiniGunRight = GetWorld()->SpawnActor<AWeaponBase>(MiniGunClass, RightMiniSpawn->GetComponentTransform(), SpawnParams);
	if (MiniGunRight)
	{
		MiniGunRight->AttachToComponent(RightMiniSpawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		MiniGunRight->SetOwner(GetOwner());
		MiniGunRight->KombatComp = this;
	}
}

void UKombatComponent::SpawnFrontLauncher()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FrontLeftLauncher = GetWorld()->SpawnActor<AWeaponBase>(LauncherClass, FrontLeftLauncherSpawn->GetComponentTransform(), SpawnParams);
	if (FrontLeftLauncher)
	{
		FrontLeftLauncher->AttachToComponent(FrontLeftLauncherSpawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		FrontLeftLauncher->SetOwner(GetOwner());
		FrontLeftLauncher->KombatComp = this;
	}

	FrontRightLauncher = GetWorld()->SpawnActor<AWeaponBase>(LauncherClass, FrontRightLauncherSpawn->GetComponentTransform(), SpawnParams);
	if (FrontRightLauncher)
	{
		FrontRightLauncher->AttachToComponent(FrontRightLauncherSpawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		FrontRightLauncher->SetOwner(GetOwner());
		FrontRightLauncher->KombatComp = this;
	}
}

void UKombatComponent::SpawnBackLauncher()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	BackLeftLauncher = GetWorld()->SpawnActor<AWeaponBase>(LauncherClass, BackLeftLauncherSpawn->GetComponentTransform(), SpawnParams);
	if (BackLeftLauncher)
	{
		BackLeftLauncher->AttachToComponent(BackLeftLauncherSpawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		BackLeftLauncher->SetOwner(GetOwner());
		BackLeftLauncher->KombatComp = this;
	}

	BackRightLauncher = GetWorld()->SpawnActor<AWeaponBase>(LauncherClass, BackRightLauncherSpawn->GetComponentTransform(), SpawnParams);
	if (BackRightLauncher)
	{
		BackRightLauncher->AttachToComponent(BackRightLauncherSpawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		BackRightLauncher->SetOwner(GetOwner());
		BackRightLauncher->KombatComp = this;
	}
}



void UKombatComponent::BeginPlay()
{
	Super::BeginPlay();

	UActorComponent* AC_LeftMini = GetOwner()->FindComponentByTag(USceneComponent::StaticClass(), FName("LeftMini"));
	if (AC_LeftMini) LeftMiniSpawn = Cast<USceneComponent>(AC_LeftMini);

	UActorComponent* AC_RightMini = GetOwner()->FindComponentByTag(USceneComponent::StaticClass(), FName("RightMini"));
	if (AC_RightMini) RightMiniSpawn = Cast<USceneComponent>(AC_RightMini);

	UActorComponent* AC_FrontLeftLauncher = GetOwner()->FindComponentByTag(USceneComponent::StaticClass(), FName("FLeftLauncher"));
	if (AC_FrontLeftLauncher) FrontLeftLauncherSpawn = Cast<USceneComponent>(AC_FrontLeftLauncher);

	UActorComponent* AC_FrontRightLauncher = GetOwner()->FindComponentByTag(USceneComponent::StaticClass(), FName("FRightLauncher"));
	if (AC_FrontRightLauncher) FrontRightLauncherSpawn = Cast<USceneComponent>(AC_FrontRightLauncher);

	UActorComponent* AC_BackLeftLauncher = GetOwner()->FindComponentByTag(USceneComponent::StaticClass(), FName("BLeftLauncher"));
	if (AC_BackLeftLauncher) BackLeftLauncherSpawn = Cast<USceneComponent>(AC_BackLeftLauncher);

	UActorComponent* AC_BackRightLauncher = GetOwner()->FindComponentByTag(USceneComponent::StaticClass(), FName("BRightLauncher"));
	if (AC_BackRightLauncher) BackRightLauncherSpawn = Cast<USceneComponent>(AC_BackRightLauncher);

	SpawnMiniGun();
	SpawnFrontLauncher();
}


void UKombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

//bool UKombatComponent::FindTarget()
//{
//	TArray<AActor*> OutActors;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAOREnemy::StaticClass(), OutActors);
//
//	float Distance = FLT_MAX;
//	AAOREnemy* Target = nullptr;
//	bool bEResult = false;
//
//	for (AActor* Item : OutActors)
//	{
//		if (Item != nullptr)
//		{
//			AAOREnemy* Enemy = Cast<AAOREnemy>(Item);
//			if (Enemy != nullptr)
//			{
//				if (!Enemy->GetIsDead())
//				{
//					double CurrentDistance = (Enemy->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
//					if (CurrentDistance < Distance)
//					{
//						Distance = CurrentDistance;
//						Target = Enemy;
//					}
//				}
//			}
//		}
//	}
//
//	CurrentProjectileTarget = Target;
//	return CurrentProjectileTarget != nullptr;
//}
