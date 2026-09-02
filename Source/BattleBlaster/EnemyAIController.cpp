// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "EnemyTank.h"


AEnemyAIController::AEnemyAIController()
{
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(RepathTimerHandle, this, &AEnemyAIController::ChasePlayer, RepathInterval, true);

}

void AEnemyAIController::ChasePlayer()
{
	AEnemyTank* EnemyTank = Cast<AEnemyTank>(GetPawn());

	if (EnemyTank && EnemyTank->Tank)
	{
		MoveToActor(EnemyTank->Tank, AcceptanceRadius);
	}


}

