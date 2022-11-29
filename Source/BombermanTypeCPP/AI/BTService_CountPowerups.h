// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CountPowerups.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UBTService_CountPowerups : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_CountPowerups(const FObjectInitializer& ObjectInitializer);

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	class AMapGrid* GetGrid();

private:
	class AMapGrid* Grid;
};
