// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BombermanTypeCPP/GAS/GAS_GameplayAbility.h"
#include "GA_BombExplode.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UGA_BombExplode : public UGAS_GameplayAbility
{
    GENERATED_BODY()

        virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
            const FGameplayAbilityActorInfo* ActorInfo,
            const FGameplayAbilityActivationInfo ActivationInfo,
            const FGameplayEventData* TriggerEventData) override;


public:     
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prereqs")
    TSubclassOf< UGameplayEffect > BombBackEffect;

private:
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
        TSubclassOf<AActor> FireDataActor;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
        FGameplayAttribute BombLengthAttribute;

    virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

};
