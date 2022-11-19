// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BombermanTypeCPP/GAS/GAS_AbilitySystemComponent.h"
#include "BombermanTypeCPP/GAS/GAS_GameplayAbility.h"
#include "BombermanTypeCPP/GAS/GAS_AttributeSet.h"

#include "BTTask_PlantBomb.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UBTTask_PlantBomb : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_PlantBomb(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	class AMapGrid* GetGrid();

	int32 index = 0;

private:
	UPROPERTY()
		class AMapGrid* Grid;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		TSubclassOf<class UGameplayAbility> ExplosionAbility;
};
