// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CountBreakables.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "Kismet/GameplayStatics.h"

UBTService_CountBreakables::UBTService_CountBreakables(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Count Reachable Powerups";
}

void UBTService_CountBreakables::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	
;
	int BreakableCount = Grid->GetBreakableCount();

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Int>("BreakableCount", BreakableCount);
}


AMapGrid* UBTService_CountBreakables::GetGrid()
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



