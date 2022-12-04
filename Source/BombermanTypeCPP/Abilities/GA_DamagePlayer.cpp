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
        //The idea here is the same explosion should not apply damage to the same character
        //Affected Players are assumed to not have the same character twice
        //So we start from the count of already affected players
        //Then apply the damage effect, and store the new affected character count
        int AffNum = FireData->AffectedPlayers.Num();
        for (int i = FireData->AffectedCount; i < AffNum; i++)
        {
            auto Context = ActorInfo->AbilitySystemComponent->MakeEffectContext();
            Context.AddSourceObject(FireData);

            FireData->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToTarget(
                DamageEffect, FireData->AffectedPlayers[i]->GetAbilitySystemComponent(), 1, Context);
        }
        FireData->SetAffectedPlayerCount(AffNum);
    }


    EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
