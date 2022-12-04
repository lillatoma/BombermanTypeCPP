// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CountPowerups.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "Kismet/GameplayStatics.h"

UBTService_CountPowerups::UBTService_CountPowerups(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Count Reachable Powerups";
}

void UBTService_CountPowerups::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//This service counts the reachable powerups

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
	TArray<FIntPoint> ReachablePoints = Grid->GetReachablePoints(GridPoint);
	int Powerups = Grid->GetReachablePowerupList(ReachablePoints).Num();

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Int>("Powerups", Powerups);
}


AMapGrid* UBTService_CountPowerups::GetGrid()
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

