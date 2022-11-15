// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveTowards.h"
#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_MoveTowards::UBTTask_MoveTowards(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Move Towards";
}

EBTNodeResult::Type UBTTask_MoveTowards::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!MyController || !MyBlackboard)
		return EBTNodeResult::Failed;

	ABombermanPlayer* Chr = Cast<ABombermanPlayer>(MyController->GetPawn());
	if (!Chr)
		return EBTNodeResult::Failed;

	UBlackboardKeyType_Vector::FDataType Location = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>("MoveToLocation");
	
	FVector Delta = Location - Chr->GetActorLocation();
	
	

	return EBTNodeResult::Succeeded;



}
