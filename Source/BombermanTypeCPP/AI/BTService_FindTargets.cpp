// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindTargets.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "Kismet/GameplayStatics.h"

UBTService_FindTargets::UBTService_FindTargets(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Find Targets";
}

void UBTService_FindTargets::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//This service counts the enemy players that are reachable

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

	FIntPoint GridPoint = Grid->GetClosestGridPoint(Chr->GetActorLocation());
	TArray<FIntPoint> ReachablePoints = Grid->GetReachablePoints(GridPoint, true);
	
	TArray<FIntPoint> Targets = Grid->GetReachablePlayerList(ReachablePoints, Chr->GetCharacterIndex());

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Int>("TargetsVisible", Targets.Num());
}


AMapGrid* UBTService_FindTargets::GetGrid()
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



