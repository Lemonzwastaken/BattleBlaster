// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Tank.h"

#include "EnemyTank.generated.h"



UCLASS()
class BATTLEBLASTER_API AEnemyTank : public ABasePawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Sets default values for this pawn's properties
	AEnemyTank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* MovementComp;

	UPROPERTY(EditAnywhere)
	float HullTurnSpeed = 5.0f;

	ATank* Tank;

	void CheckFireCondition();
	bool IsInFireRange();
	void HandleDestruction();

};
