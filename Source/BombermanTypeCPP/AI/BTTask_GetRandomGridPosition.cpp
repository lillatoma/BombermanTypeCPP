// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomGridPosition.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

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
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!MyController || !MyBlackboard)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No controller or blackboard")));
		return EBTNodeResult::Failed;
	}
	ABombermanPlayer* Chr = Cast<ABombermanPlayer>(MyController->GetPawn());
	if (!Chr || !GetGrid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No character or grid")));
		return EBTNodeResult::Failed;
	}
	FIntPoint ChrOnGrid = Grid->GetClosestGridPoint(Chr->GetActorLocation());

	if (!Grid->IsPointOnGrid(ChrOnGrid))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Not on grid")));
		return EBTNodeResult::Failed;
	}
	TArray<FIntPoint> PossiblePoints = Grid->GetReachablePoints(ChrOnGrid);

	if(PossiblePoints.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Nowhere to go")));
		return EBTNodeResult::Failed;
	}

	FIntPoint RandomPoint = PossiblePoints[FMath::RandRange(0, PossiblePoints.Num() - 1)];
	FVector point = Grid->ConvertGridToWorld(RandomPoint);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Total points: %d, selected: %d|%d"),PossiblePoints.Num(), RandomPoint.X, RandomPoint.Y));

	if (Grid->IsSafePoint(RandomPoint))
	{

		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", point);

		return EBTNodeResult::Succeeded;
	}
	else return EBTNodeResult::Failed;


}
