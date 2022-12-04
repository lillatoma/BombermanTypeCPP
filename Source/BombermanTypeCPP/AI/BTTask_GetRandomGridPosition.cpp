// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomGridPosition.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

#include "Kismet/GameplayStatics.h"




UBTTask_GetRandomGridPosition::UBTTask_GetRandomGridPosition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Random Safe Grid Position";
}

AMapGrid* UBTTask_GetRandomGridPosition::GetGrid()
{
	if (Grid)
		return Grid;

	AActor* GridActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMapGrid::StaticClass());
	if (GridActor)
	{
		AMapGrid* GridB = Cast<AMapGrid>(GridActor);
		if (GridB)
		{
			Grid = GridB;
			return Grid;
		}
	}
	return nullptr;

}

EBTNodeResult::Type UBTTask_GetRandomGridPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//This task randomly finds a safe position on the grid
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!MyController || !MyBlackboard)
	{
		return EBTNodeResult::Failed;
	}
	ABombermanPlayer* Chr = Cast<ABombermanPlayer>(MyController->GetPawn());
	if (!Chr || !GetGrid())
	{
		return EBTNodeResult::Failed;
	}
	FIntPoint ChrOnGrid = Grid->GetClosestGridPoint(Chr->GetActorLocation());

	if (!Grid->IsPointOnGrid(ChrOnGrid))
	{
		return EBTNodeResult::Failed;
	}
	TArray<FIntPoint> PossiblePoints = Grid->GetSafeReachablePoints(ChrOnGrid);

	if (PossiblePoints.Num() <= 1)
	{
		return EBTNodeResult::Failed;
	}

	FIntPoint RandomPoint = PossiblePoints[FMath::RandRange(0, PossiblePoints.Num() - 1)];
	FVector point = Grid->ConvertGridToWorld(RandomPoint);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Total points: %d, selected: %d|%d"),PossiblePoints.Num(), RandomPoint.X, RandomPoint.Y));

	if (Grid->IsSafePoint(RandomPoint))
	{

		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", point);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("MoveToLocationSafe", true);
		return EBTNodeResult::Succeeded;
	}
	else return EBTNodeResult::Failed;


}
