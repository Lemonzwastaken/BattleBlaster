// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTank.h"

// Sets default values
AEnemyTank::AEnemyTank()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyTank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

