// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "Tank.h"

#include "EnemyTank.generated.h"

UCLASS()
class BATTLEBLASTER_API AEnemyTank : public ABasePawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AEnemyTank();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* MovementComp;

	UPROPERTY(EditAnywhere)
	float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere)
	float FireRate = 2.0f;

	UPROPERTY(EditAnywhere)
	float HullTurnSpeed = 5.0f;

	ATank* Tank;

	void CheckFireCondition();
	bool IsInFireRange();
	void HandleDestruction();
};