// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_BombExplode.h"
#include "BombermanTypeCPP\GAS\Gas_AbilitySystemComponent.h"
#include "BombermanTypeCPP\Game\BombermanBomb.h"
#include "BombermanTypeCPP\Game\BombermanPlayer.h"
#include "BombermanTypeCPP\Game\BombermanFireDataHolder.h"




void UGA_BombExplode::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Activate Ability")));

    ABombermanBomb* Bomb = Cast<ABombermanBomb>(ActorInfo->OwnerActor);

    if (Bomb)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bomb exists")));
        if (Bomb->OriginalPlayer)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bomb's player exists")));
            auto Context = ActorInfo->AbilitySystemComponent->MakeEffectContext();
            Context.AddSourceObject(Bomb);

            Bomb->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToTarget(
                BombBackEffect, Bomb->OriginalPlayer->GetAbilitySystemComponent(), 1, Context);
            //Bomb->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(BombBackEffect, OriginalPlayer->GetAbilitySystemComponent());
        
            //Spawning FireDataHolder
            FVector Location = ActorInfo->OwnerActor->GetActorLocation();
            const FRotator Rotation = FRotator();

            FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
            SpawnParameters.Owner = Bomb->OriginalPlayer;

            AActor* FireDataObject = GetWorld()->SpawnActor<AActor>(FireDataActor, Location, Rotation, SpawnParameters);


            if (FireDataObject)
            {
                ABombermanFireDataHolder* FireData = Cast<ABombermanFireDataHolder>(FireDataObject);

                if (FireData)
                {
                    bool bFound = false;
                    float bombLength = Bomb->OriginalPlayer->GetAbilitySystemComponent()
                        ->GetGameplayAttributeValue(BombLengthAttribute, bFound);
                    FireData->ExplosionLength = bombLength;
                }


            }






        }
    }
    EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UGA_BombExplode::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);
}

