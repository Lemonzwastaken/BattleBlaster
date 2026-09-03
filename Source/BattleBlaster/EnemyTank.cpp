#include "EnemyTank.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"

AEnemyTank::AEnemyTank()
{
	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(
		TEXT("MovementComp")
	);

	MovementComp->SetUpdatedComponent(CapsuleComp);

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyTank::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)
	);

	FTimerHandle FireTimerHandle;

	GetWorldTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&AEnemyTank::CheckFireCondition,
		FireRate,
		true
	);
}

void AEnemyTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Tank)
	{
		Tank = Cast<ATank>(
			UGameplayStatics::GetPlayerPawn(GetWorld(), 0)
		);
	}

	if (!Tank || !Tank->IsAlive || !MovementComp)
	{
		return;
	}

	FVector DirectionToTank =
		Tank->GetActorLocation() - GetActorLocation();

	DirectionToTank.Z = 0.0f;

	const float DistanceToTank = DirectionToTank.Size();

	FollowNavigationPath();

	RotateTurret(Tank->GetActorLocation());

	const FVector Velocity = MovementComp->Velocity;

	if (!Velocity.IsNearlyZero())
	{
		const FRotator TargetRotation(
			0.0f,
			Velocity.Rotation().Yaw,
			0.0f
		);

		SetActorRotation(
			FMath::RInterpTo(
				GetActorRotation(),
				TargetRotation,
				DeltaTime,
				HullTurnSpeed
			)
		);
	}
}

UPawnMovementComponent* AEnemyTank::GetMovementComponent() const
{
	return MovementComp;
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
	if (!Tank)
	{
		return false;
	}

	return FVector::Dist(
		GetActorLocation(),
		Tank->GetActorLocation()
	) <= FireRange;
}

void AEnemyTank::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void AEnemyTank::SetNavigationPath(const TArray<FVector>& NewPathPoints)
{
	NavigationPathPoints = NewPathPoints;
	CurrentPathPointIndex = 0;
}

bool AEnemyTank::FollowNavigationPath()
{
	if (!MovementComp || NavigationPathPoints.IsEmpty())
	{
		return false;
	}

	while (CurrentPathPointIndex < NavigationPathPoints.Num())
	{
		FVector DirectionToWaypoint =
			NavigationPathPoints[CurrentPathPointIndex] - GetActorLocation();
		DirectionToWaypoint.Z = 0.0f;

		if (DirectionToWaypoint.SizeSquared() <=
			FMath::Square(PathPointAcceptanceRadius))
		{
			++CurrentPathPointIndex;
			continue;
		}

		MovementComp->AddInputVector(DirectionToWaypoint.GetSafeNormal(), true);
		return true;
	}

	return false;
}
