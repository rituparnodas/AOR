// Fill out your copyright notice in the Description page of Project Settings.


#include "AIWeapon.h"

// Sets default values
AAIWeapon::AAIWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAIWeapon::Shoot()
{
}

void AAIWeapon::StopShot()
{
}

void AAIWeapon::ShootLogic()
{
}

void AAIWeapon::ApplyDamage(FHitResult HitResult)
{
}

// Called when the game starts or when spawned
void AAIWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

