// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckSafety.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UBTService_CheckSafety : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_CheckSafety(const FObjectInitializer& ObjectInitializer);

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	class AMapGrid* GetGrid();

private:
	class AMapGrid* Grid;
};
