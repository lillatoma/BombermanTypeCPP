// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_DamagePlayer.h"
#include "BombermanTypeCPP\Game\BombermanFireDataHolder.h"
#include "BombermanTypeCPP\Game\BombermanPlayer.h"

void UGA_DamagePlayer::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ABombermanFireDataHolder* FireData = Cast<ABombermanFireDataHolder>(ActorInfo->OwnerActor);

    if (FireData)
    {
        for (int i = FireData->AffectedCount; i < FireData->AffectedPlayers.Num(); i++)
        {
            auto Context = ActorInfo->AbilitySystemComponent->MakeEffectContext();
            Context.AddSourceObject(FireData);

            FireData->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToTarget(
                DamageEffect, FireData->AffectedPlayers[i]->GetAbilitySystemComponent(), 1, Context);
        }
        FireData->SetAffectedPlayerCount(FireData->AffectedPlayers.Num());
    }


    EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
