// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindBombPlacePosition.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UBTTask_FindBombPlacePosition : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTTask_FindBombPlacePosition(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	class AMapGrid* GetGrid();

	int32 index = 0;

private:
	UPROPERTY()
		class AMapGrid* Grid;

};