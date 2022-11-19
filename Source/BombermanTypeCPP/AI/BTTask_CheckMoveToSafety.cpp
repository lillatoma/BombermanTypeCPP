// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckMoveToSafety.h"

#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGrid.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

#include "Kismet/GameplayStatics.h"

UBTTask_CheckMoveToSafety::UBTTask_CheckMoveToSafety(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Check MoveTo Safety";
}

AMapGrid* UBTTask_CheckMoveToSafety::GetGrid()
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

EBTNodeResult::Type UBTTask_CheckMoveToSafety::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UBlackboardKeyType_Vector::FDataType Location = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>("MoveToLocation");

	FIntPoint MoveToPoint = Grid->GetClosestGridPoint(Location);

	if (Grid->IsSafePoint(MoveToPoint))
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("MoveToLocationSafe", true);
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Not Safe, selected: %d|%d"), MoveToPoint.X, MoveToPoint.Y));
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("MoveToLocationSafe", false);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation",Chr->GetActorLocation());
	}
	return EBTNodeResult::Succeeded;



}


