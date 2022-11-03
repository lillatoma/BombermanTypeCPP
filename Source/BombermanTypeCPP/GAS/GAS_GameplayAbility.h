// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BombermanTypeCPP.h"

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GAS_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UGAS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGAS_GameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
};
