// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveTowards.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UBTTask_MoveTowards : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
		
public:
	UBTTask_MoveTowards(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	int32 index = 0;
};
