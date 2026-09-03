#include "EnemyAIController.h"
#include "EnemyTank.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

AEnemyAIController::AEnemyAIController()
{
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		RepathTimerHandle,
		this,
		&AEnemyAIController::ChasePlayer,
		RepathInterval,
		true
	);
}

void AEnemyAIController::ChasePlayer()
{

	AEnemyTank* EnemyTank = Cast<AEnemyTank>(GetPawn());

	ATank* PlayerTank = Cast<ATank>(
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)
	);

	if (!EnemyTank || !PlayerTank)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Enemy valid: %s | Player tank valid: %s"),
			EnemyTank ? TEXT("true") : TEXT("false"),
			PlayerTank ? TEXT("true") : TEXT("false")
		);
		return;
	}

	EnemyTank->Tank = PlayerTank;

	UNavigationPath* NavigationPath =
		UNavigationSystemV1::FindPathToActorSynchronously(
			this,
			EnemyTank->GetActorLocation(),
			PlayerTank,
			50.0f,
			EnemyTank
		);

	if (NavigationPath && NavigationPath->IsValid() &&
		NavigationPath->PathPoints.Num() > 1)
	{
		EnemyTank->SetNavigationPath(NavigationPath->PathPoints);
		UE_LOG(LogTemp, Warning, TEXT("Navigation path found: %d points"),
			NavigationPath->PathPoints.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid navigation path to player"));
	}
}
