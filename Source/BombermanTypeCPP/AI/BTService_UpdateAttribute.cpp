// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateAttribute.h"
#include "BombermanAIController.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

UBTService_UpdateAttribute::UBTService_UpdateAttribute(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Update Attribute";
}

void UBTService_UpdateAttribute::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//This service is a base for updating a specific attribute

	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!MyController || !MyBlackboard)
	{
		return;
	}
	ABombermanPlayer* Chr = Cast<ABombermanPlayer>(MyController->GetPawn());
	if (!Chr)
	{
		return;
	}

	if (Chr->GetAbilitySystemComponent())
	{
		bool bFound = false;

		float AttributeValue = Chr->GetAbilitySystemComponent()->GetGameplayAttributeValue(Attribute, bFound);

		if (bFound)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Float>(AttributeName, AttributeValue);
		}
	}

}
