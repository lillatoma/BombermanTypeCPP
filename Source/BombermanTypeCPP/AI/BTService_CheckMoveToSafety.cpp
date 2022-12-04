// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckMoveToSafety.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckMoveToSafety::UBTService_CheckMoveToSafety(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Check MoveTo Safety From Bombs";
}

void UBTService_CheckMoveToSafety::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//This service checks if the MoveToLocation is safe from bombs

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

	UBlackboardKeyType_Vector::FDataType Location = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>("MoveToLocation");

	FIntPoint MoveToPoint = Grid->GetClosestGridPoint(Location);

	bool Safe = Grid->IsSafePoint(MoveToPoint);


	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("MoveToLocationSafe", Safe);
}


AMapGrid* UBTService_CheckMoveToSafety::GetGrid()
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