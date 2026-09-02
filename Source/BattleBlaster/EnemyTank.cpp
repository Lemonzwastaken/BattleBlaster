// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyTank.h"
#include "EnemyAIController.h"

AEnemyTank::AEnemyTank()
{
	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyTank::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AEnemyTank::CheckFireCondition, FireRate, true);
}

void AEnemyTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}

	FVector Velocity = MovementComp->Velocity;
	if (!Velocity.IsNearlyZero())
	{
		FRotator TargetRotation = FRotator(0.0f, Velocity.Rotation().Yaw, 0.0f);
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, HullTurnSpeed);
		SetActorRotation(NewRotation);
	}
}

void AEnemyTank::CheckFireCondition()
{
	if (Tank && Tank->IsAlive && IsInFireRange())
	{
		Fire();
	}
}

bool AEnemyTank::IsInFireRange()
{
	bool Result = false;

	if (Tank)
	{
		float DistanceToTank = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		Result = (DistanceToTank <= FireRange);
	}

	return Result;
}

void AEnemyTank::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}