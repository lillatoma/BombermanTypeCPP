// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BombermanTypeCPP/GAS/GAS_GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GA_PlantBomb.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayMontageAndWaitForEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UGA_PlantBomb : public UGAS_GameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayTagContainer* SourceTags = nullptr,
        const FGameplayTagContainer* TargetTags = nullptr,
        OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

private:
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
        TSubclassOf<AActor> BombActor;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
        FGameplayAttribute BombCountAttribute;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
        TSubclassOf< UGameplayEffect > BombBackEffect;


    void OnAnimNotifyBegin(FGameplayTag Tag, const FGameplayEventData* Payload);

    FGameplayAbilitySpecHandle StoredHandle;
    const FGameplayAbilityActorInfo* StoredActorInfo;
    FGameplayAbilityActivationInfo StoredActivationInfo;
    const FGameplayEventData* StoredTriggerEventData;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
        class UAnimMontage* PlantMontage;

    UPROPERTY()
        FGameplayTagContainer EventTags;

    FOnMontageBlendingOutStarted BlendingOutDelegate;
    FOnMontageEnded MontageEndedDelegate;
    FDelegateHandle CancelledHandle;
    FDelegateHandle EventHandle;

    void FinishAbility(UAnimMontage* Montage, bool bInterrupted);
};
