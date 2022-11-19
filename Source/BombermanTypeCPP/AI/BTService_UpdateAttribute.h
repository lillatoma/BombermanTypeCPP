// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "BombermanTypeCPP/GAS/GAS_AbilitySystemComponent.h"
#include "BombermanTypeCPP/GAS/GAS_GameplayAbility.h"
#include "BombermanTypeCPP/GAS/GAS_AttributeSet.h"
#include "BTService_UpdateAttribute.generated.h"



/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UBTService_UpdateAttribute : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateAttribute(const FObjectInitializer& ObjectInitializer);
	
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		FGameplayAttribute Attribute;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		FName AttributeName;
};
