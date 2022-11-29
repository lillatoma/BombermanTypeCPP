// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttackPosition.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckAttackPosition::UBTService_CheckAttackPosition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Check Attack Position";
}

void UBTService_CheckAttackPosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!MyController || !MyBlackboard)
	{
		return;
	}
	ABombermanPlayer* Chr = Cast<ABombermanPlayer>(MyController->GetPawn());
	if (!Chr || !GetGrid())
	{
		return;
	}

	float BombRange = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Float>("BombRange");
	FVector OriginalTarget = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>("TargetLocation");
	FIntPoint TargetPoint = Grid->GetClosestGridPoint(OriginalTarget);

	FIntPoint GridPoint = Grid->GetClosestGridPoint(Chr->GetActorLocation());
	TArray<FIntPoint> ReachablePoints = Grid->GetReachablePoints(GridPoint, true);

	TArray<FIntPoint> Targets = Grid->GetReachablePlayerList(ReachablePoints, Chr->GetCharacterIndex());

	bool Found = false;

	for (int i = 0; i < Targets.Num(); i++)
	{
		TArray<FIntPoint> OverseePoints = Grid->GetPointsOverseeing(Targets[i], BombRange);
		if (OverseePoints.Find(TargetPoint) != INDEX_NONE)
		{
			Found = true;
			break;
		}
	}

	if (!Found)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("TargetPositionValid", false);
	}



}


AMapGrid* UBTService_CheckAttackPosition::GetGrid()
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





