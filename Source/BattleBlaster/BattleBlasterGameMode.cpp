// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "StartScreen.h"

#include "BattleBlasterGameInstance.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Display, TEXT("Number of towers: %d"), TowerCount);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Display, TEXT("GameMode: Failed to find the tank actor!"));
		}
	}

	int32 LoopIndex = 0;
	while (LoopIndex < TowerCount)
	{
		AActor* TowerActor = Towers[LoopIndex];
		if (TowerActor)
		{
			ATower* Tower = Cast<ATower>(TowerActor);
			if (Tower && Tank)
			{
				Tower->Tank = Tank;
				UE_LOG(LogTemp, Display, TEXT("%s setting the tank variable!"), *Tower->GetActorNameOrLabel());
			}
		}

		LoopIndex++;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		StartScreenWidget = CreateWidget<UStartScreen>(PlayerController, StartScreenClass);
		if (StartScreenWidget)
		{
			StartScreenWidget->AddToPlayerScreen();
			StartScreenWidget->OnFadeOutComplete.AddDynamic(this, &ABattleBlasterGameMode::OnStartScreenFadedOut);
		}

		IntroCameraTarget = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
		if (IntroCameraTarget)
		{
			PlayerController->SetViewTargetWithBlend(IntroCameraTarget, 0.0f);
		}
	}
}

void ABattleBlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGameStarted)
	{
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController && PlayerController->WasInputKeyJustPressed(EKeys::Enter))
	{
		StartGame();
	}

}

ABattleBlasterGameMode::ABattleBlasterGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABattleBlasterGameMode::StartGame()
{
	bGameStarted = true;

	if (StartScreenWidget)
	{
		StartScreenWidget->PlayFadeOut();
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController && Tank)
	{
		PlayerController->SetViewTargetWithBlend(Tank, CameraBlendTime);
	}

}


void ABattleBlasterGameMode::OnCountDownTimerTimeout()
{

	CountdownSeconds -= 1;

	if (CountdownSeconds > 0)
	{
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));

	}
	else if (CountdownSeconds == 0)
	{
		ScreenMessageWidget->SetMessageText("GO!");
		Tank->SetPlayerEnabled(true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}

}

void ABattleBlasterGameMode::OnStartScreenFadedOut()
{	
	GetWorldTimerManager().SetTimer(PostFadeTimerHandle, this, &ABattleBlasterGameMode::ShowGetReadyMessage, 1, false);
}

void ABattleBlasterGameMode::ShowGetReadyMessage()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready");
		}
	}

	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountDownTimerTimeout, 1.0f, true);
}



void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		IsGameOver = true;
	}
	else
	{
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower)
		{
			DeadTower->HandleDestruction();

			TowerCount--;
			if (TowerCount == 0)
			{
				IsGameOver = true;
				IsVictory = true;
			}
		}
	}

	if (IsGameOver)
	{
		FString GameOverString = IsVictory ? "Victory!" : "Defeat!";

		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);
		ScreenMessageWidget->SetMessageText(GameOverString);

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance)
		{
			if (IsVictory)
			{
				// Load the next level
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				// Reload the current level
				UGameplayStatics::OpenLevel(GetWorld(), *CurrentLevelName);
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}
}
