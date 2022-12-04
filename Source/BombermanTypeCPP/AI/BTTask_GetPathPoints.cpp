// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetPathPoints.h"
#include "BombermanAIController.h"
#include "Components/SplineComponent.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_GetPathPoints::UBTTask_GetPathPoints(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Path Points";
}

EBTNodeResult::Type UBTTask_GetPathPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Archive
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!MyController || !MyBlackboard)
		return EBTNodeResult::Failed;

	ABombermanPlayer* Chr = Cast<ABombermanPlayer>(MyController->GetPawn());
	if (!Chr)
		return EBTNodeResult::Failed;

	APatrolPath* PathRef = Cast<APatrolPath>(Chr->SmartObject);
	if (!PathRef || PathRef->Locations.Num() < 1)
		return EBTNodeResult::Succeeded;

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", PathRef->Locations[index]);

	if (index < PathRef->Locations.Num() - 1)
	{
		index++;
		return EBTNodeResult::Succeeded;
	}
	index = 0;
	return EBTNodeResult::Succeeded;



}
