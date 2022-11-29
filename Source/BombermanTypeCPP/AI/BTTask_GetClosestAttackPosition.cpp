// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetClosestAttackPosition.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

#include "Kismet/GameplayStatics.h"

UBTTask_GetClosestAttackPosition::UBTTask_GetClosestAttackPosition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Closest Attack Position";
}

AMapGrid* UBTTask_GetClosestAttackPosition::GetGrid()
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

EBTNodeResult::Type UBTTask_GetClosestAttackPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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

	TArray<FIntPoint> ReachablePoints = Grid->GetReachablePoints(ChrOnGrid, true);
	TArray<FIntPoint> TargetPoints = Grid->GetReachablePlayerList(ReachablePoints,Chr->GetCharacterIndex());

	if (TargetPoints.Num() == 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Int>("TargetsVisible", 0);
		return EBTNodeResult::Failed;
	}
	else
	{
		TArray<FIntPoint> ReachPoints;

		float BombRange = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Float>("BombRange");

		for (int i = 0; i < TargetPoints.Num(); i++)
		{
			TArray<FIntPoint> OverseePoints = Grid->GetPointsOverseeing(TargetPoints[i], BombRange);
			OverseePoints = AMapGrid::Intersection(OverseePoints, ReachablePoints);
			int TNum = OverseePoints.Num();

			for (int j = 0; j < TNum; j++)
				ReachPoints.Add(OverseePoints[j]);

		}

		if (ReachPoints.Num() == 0)
			return EBTNodeResult::Failed;

		int Index = 0;
		double Distance = FVector::Dist2D(Chr->GetActorLocation(), Grid->ConvertGridToWorld(ReachPoints[0]));
		int Num = ReachPoints.Num();
		for (int i = 1; i < Num; i++)
		{
			double Distance2 = FVector::Dist2D(Chr->GetActorLocation(), Grid->ConvertGridToWorld(ReachPoints[i]));

			if (Distance2 < Distance)
			{
				Index = i;
				Distance = Distance2;
			}
		}

		FVector Location = Grid->ConvertGridToWorld(ReachPoints[Index]);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("TargetLocation", Location);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("TargetPositionValid", true);
	}


	return EBTNodeResult::Succeeded;



}






