// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindClosestPowerup.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"

#include "Kismet/GameplayStatics.h"

UBTTask_FindClosestPowerup::UBTTask_FindClosestPowerup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Find Closest Powerup";
}

AMapGrid* UBTTask_FindClosestPowerup::GetGrid()
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

EBTNodeResult::Type UBTTask_FindClosestPowerup::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//This tasks finds the closest reachable powerup
	//TODO: Use grid to count distance, instead of linear distance

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

	TArray<FIntPoint> ReachablePoints = Grid->GetReachablePoints(ChrOnGrid);
	TArray<FIntPoint> PowerupPoints = Grid->GetReachablePowerupList(ReachablePoints);

	if (PowerupPoints.Num() == 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Int>("Powerups", 0);
		return EBTNodeResult::Failed;
	}
	else 
	{
		if (PowerupPoints.Num() == 1)
		{
			FVector Location = Grid->ConvertGridToWorld(PowerupPoints[0]);
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation",Location);
		}
		else
		{
			int Index = 0;
			double Distance = FVector::Dist2D(Chr->GetActorLocation(), Grid->ConvertGridToWorld(PowerupPoints[0]));
			int Num = PowerupPoints.Num();
			for (int i = 1; i < Num; i++)
			{
				double Distance2 = FVector::Dist2D(Chr->GetActorLocation(), Grid->ConvertGridToWorld(PowerupPoints[i]));

				if (Distance2 < Distance)
				{
					Index = i;
					Distance = Distance2;
				}
			}

			FVector Location = Grid->ConvertGridToWorld(PowerupPoints[Index]);
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", Location);
		}
	}


	return EBTNodeResult::Succeeded;



}




