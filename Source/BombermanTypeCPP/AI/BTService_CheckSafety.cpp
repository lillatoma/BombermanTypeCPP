// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckSafety.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckSafety::UBTService_CheckSafety(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Check Current Safety From Bombs";
}

void UBTService_CheckSafety::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//This services checks if the current location is safe from bombs
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

	bool Safe = Grid->IsSafePoint(GridPoint);

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("CurrentSafe", Safe);
}


AMapGrid* UBTService_CheckSafety::GetGrid()
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