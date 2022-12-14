// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BombermanTypeCPP/GAS/GAS_GameplayAbility.h"
#include "GA_DamagePlayer.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UGA_DamagePlayer : public UGAS_GameplayAbility
{
	GENERATED_BODY()
	
		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
			const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo,
			const FGameplayEventData* TriggerEventData) override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		TSubclassOf< UGameplayEffect > DamageEffect;
};
