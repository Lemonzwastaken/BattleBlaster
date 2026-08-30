// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

#include "Tank.generated.h"

class UCameraComponent;

/**
 *
 */
UCLASS()
class BATTLEBLASTER_API ATank : public ABasePawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ATank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurretTurnAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* BrakeAction;

	UPROPERTY(EditAnywhere)
	float TurretTurnRate = 100.0f;

	UPROPERTY(EditAnywhere)
	float Acceleration = 500.0f;

	float TargetMoveInput = 0.0f;
	float CurrentMoveInput = 0.0f;

	UPROPERTY(EditAnywhere)
	float BrakeDeceleration = 1500.0f;
	bool bIsBraking = false;

	void BrakeInput(const FInputActionValue& Value);
	void BrakeInputCompleted(const FInputActionValue& Value);

	void TurretTurnInput(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere)
	float TurnRate = 50.0f;

	APlayerController* PlayerController;

	bool IsAlive = true;

	void MoveInput(const FInputActionValue& Value);
	void MoveInputCompleted(const FInputActionValue& Value);
	void TurnInput(const FInputActionValue& Value);

	void HandleDestruction();
	void SetPlayerEnabled(bool Enabled);
};
