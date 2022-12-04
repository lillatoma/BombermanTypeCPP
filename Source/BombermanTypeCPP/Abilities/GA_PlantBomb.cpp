// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_PlantBomb.h"
#include "BombermanTypeCPP\GAS\Gas_AbilitySystemComponent.h"
#include "BombermanTypeCPP\Game\BombermanBomb.h"
#include "BombermanTypeCPP\Game\BombermanPlayer.h"
#include "BombermanTypeCpp\Map\MapGrid.h"
//#include "Abilities\AbilityTasks\GDAT_PlayMontageAndWaitForEvent.h"

#include "Kismet/GameplayStatics.h"

void UGA_PlantBomb::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    StoredHandle = Handle;
    StoredActorInfo = ActorInfo;
    StoredActivationInfo = ActivationInfo;
    StoredTriggerEventData = TriggerEventData;

    ABombermanPlayer* Player = nullptr;
    if (ActorInfo)
        Player = Cast<ABombermanPlayer>(ActorInfo->OwnerActor);

    if (Player && ActorInfo->GetAnimInstance())
    {
        float MontageLength = Player->GetAbilitySystemComponent()->PlayMontage(this, ActivationInfo, PlantMontage, 1.f);


        if (MontageLength > 0 && ActorInfo->GetAnimInstance())
        {
            //In case, the animation triggers a gameplay event, we handle it with OnAnimNotifyBegin
            Player->GetAbilitySystemComponent()->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UGA_PlantBomb::OnAnimNotifyBegin));

            //This is probably not needed
            BlendingOutDelegate.BindUObject(this, &UGA_PlantBomb::FinishAbility);
            ActorInfo->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendingOutDelegate, PlantMontage);

            //If the animation ends before the event, end the ability
            MontageEndedDelegate.BindUObject(this, &UGA_PlantBomb::FinishAbility);
            ActorInfo->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, PlantMontage);
        }

    }
    else
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);

}


void UGA_PlantBomb::OnAnimNotifyBegin(FGameplayTag Tag, const FGameplayEventData* Payload)
{
    //Check if the Gameplay Event is the one with PlantBomb tag
    FName Name = Tag.GetTagName();

    if (Name.Compare(FName("PlantBomb"))) 
        return;

    FVector Location = StoredActorInfo->OwnerActor->GetActorLocation();
    const FRotator Rotation = FRotator();

    //Finding Grid
    AActor* GridActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMapGrid::StaticClass());
    AMapGrid* Grid = nullptr;
    FIntPoint BombPoint;


    if (GridActor)
    {
        Grid = Cast<AMapGrid>(GridActor);
        if (Grid)
        {
            //Correcting Location to be on the grid
            Location = Grid->GetClosestGridCenter(Location);
            BombPoint = Grid->GetClosestGridPoint(Location);

            //In case there is a bomb at that position, we don't plant bomb, and give back the ability cost.
            if (Grid->IsBombOnPoint(BombPoint))
            {
                ABombermanPlayer* Player = Cast<ABombermanPlayer>(StoredActorInfo->OwnerActor);
                if (Player)
                {
                    auto Context = StoredActorInfo->AbilitySystemComponent->MakeEffectContext();
                    Context.AddSourceObject(Player);
                    Player->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToTarget(
                        BombBackEffect, Player->GetAbilitySystemComponent(), 1, Context);

                    EndAbility(StoredHandle, StoredActorInfo, StoredActivationInfo, false, true);
                    return;
                }
            }
        }
    }


    //Spawning the bomb
    //If there is a grid, it adds bomb to the grid
    //That makes sure that there can't be two bombs in one place.
    AActor* BombObject = GetWorld()->SpawnActor<AActor>(BombActor, Location, Rotation);

    
    ABombermanBomb* Bomb = Cast<ABombermanBomb>(BombObject);
    if (Bomb)
    {
        ABombermanPlayer* Player = Cast<ABombermanPlayer>(StoredActorInfo->OwnerActor);
        if (Player)
        {
            Bomb->OriginalPlayer = Player;
        }

        //This bool makes sure the bomb doesn't explode right on spawn
        Bomb->HasBeenSetUp = true;
        if (Grid)
            Grid->AddBomb(BombPoint);
    }

    EndAbility(StoredHandle, StoredActorInfo, StoredActivationInfo, false, false);
}

bool UGA_PlantBomb::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayTagContainer* SourceTags,
    const FGameplayTagContainer* TargetTags,
    FGameplayTagContainer* OptionalRelevantTags) const
{

    bool bFound = false;

    //The ability should only activate if the BombCount attribute of the player is at least 1.
    float bombCount = ActorInfo->AbilitySystemComponent->GetGameplayAttributeValue(BombCountAttribute,bFound);
    return Super::CanActivateAbility(Handle,ActorInfo,SourceTags,TargetTags,OptionalRelevantTags) 
        && bFound && bombCount > 0;
}

void UGA_PlantBomb::FinishAbility(UAnimMontage* Montage, bool bInterrupted)
{
    EndAbility(StoredHandle, StoredActorInfo, StoredActivationInfo, false, false);
}


