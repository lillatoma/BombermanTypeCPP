// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_BombExplode.h"
#include "BombermanTypeCPP\GAS\Gas_AbilitySystemComponent.h"
#include "BombermanTypeCPP\Game\BombermanBomb.h"
#include "BombermanTypeCPP\Game\BombermanPlayer.h"
#include "BombermanTypeCPP\Game\BombermanFireDataHolder.h"

#include "Kismet/GameplayStatics.h"


void UGA_BombExplode::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ABombermanBomb* Bomb = Cast<ABombermanBomb>(ActorInfo->OwnerActor);

    if (Bomb)
    {
        if (Bomb->OriginalPlayer)
        {
            //Giving back the bomb to the player, so they can plant again
            auto Context = ActorInfo->AbilitySystemComponent->MakeEffectContext();
            Context.AddSourceObject(Bomb);
            Bomb->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToTarget(
                BombBackEffect, Bomb->OriginalPlayer->GetAbilitySystemComponent(), 1, Context);
        
            //Spawning FireDataHolder
            //We need to pass the explosion's length before fully spawning
            FVector Location = ActorInfo->OwnerActor->GetActorLocation();
            const FRotator Rotation = FRotator();
            FTransform SpawnTransform(Rotation, Location, FVector(1, 1, 1));

            ABombermanFireDataHolder* FireData = Cast<ABombermanFireDataHolder>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, FireDataActor, SpawnTransform));

            if (FireData)
            {
                bool bFound = false;
                float bombLength = Bomb->OriginalPlayer->GetAbilitySystemComponent()
                    ->GetGameplayAttributeValue(BombLengthAttribute, bFound);
                if(bFound)
                    FireData->ExplosionLength = bombLength;
                UGameplayStatics::FinishSpawningActor(FireData, SpawnTransform);
            }

        }
    }
    EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

