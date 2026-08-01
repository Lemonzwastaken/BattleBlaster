// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

}


void ATank::BeginPlay()
{
	Super::BeginPlay();

}

void ATank::Tick(float DeltaTime)
{



}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{



}

