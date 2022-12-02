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

    ABombermanPlayer* Player = Cast<ABombermanPlayer>(ActorInfo->OwnerActor);

    if (Player && Player->GetMesh() && Player->GetMesh()->GetAnimInstance())
    {
        //UGDAT_PlayMontageAndWaitForEvent* Task = UGDAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
        //Task->OnBlendOut.AddDynamic(this, &UGDGA_FireGun::OnCompleted);
        //Task->OnCompleted.AddDynamic(this, &UGDGA_FireGun::OnCompleted);
        //Task->OnInterrupted.AddDynamic(this, &UGDGA_FireGun::OnCancelled);
        //Task->OnCancelled.AddDynamic(this, &UGDGA_FireGun::OnCancelled);
        //Task->EventReceived.AddDynamic(this, &UGDGA_FireGun::EventReceived);
        //Task->ReadyForActivation();


        float MontageLength = Player->GetAbilitySystemComponent()->PlayMontage(this, ActivationInfo, PlantMontage, 1.f);
        //Player->GetMesh()->GetAnimInstance()->Montage_

        //float MontageLength = Player->GetMesh()->GetAnimInstance()->Montage_Play(PlantMontage);

        if (MontageLength > 0 && ActorInfo->GetAnimInstance())
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Montage >> 0")));
            Player->GetAbilitySystemComponent()->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UGA_PlantBomb::OnAnimNotifyBeginGP));
            //Player->GetAbilitySystemComponent()->animinstt
            //ActorInfo->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UGA_PlantBomb::OnAnimNotifyBegin);

            BlendingOutDelegate.BindUObject(this, &UGA_PlantBomb::FinishAbility);
            ActorInfo->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendingOutDelegate, PlantMontage);

            MontageEndedDelegate.BindUObject(this, &UGA_PlantBomb::FinishAbility);
            ActorInfo->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, PlantMontage);
        }

    }
    else
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);

}

void UGA_PlantBomb::OnAnimNotifyBegin(FName Name, const FBranchingPointNotifyPayload& Payload)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Name.ToString());

    if (Name.Compare(FName("Plant")))
        return;

    FVector Location = StoredActorInfo->OwnerActor->GetActorLocation();
    const FRotator Rotation = FRotator();


    AActor* GridActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMapGrid::StaticClass());
    FIntPoint BombPoint;


    if (GridActor)
    {
        AMapGrid* Grid = Cast<AMapGrid>(GridActor);
        if (Grid)
        {
            Location = Grid->GetClosestGridCenter(Location);
            BombPoint = Grid->GetClosestGridPoint(Location);

            if (Grid->IsBombOnPoint(BombPoint))
            {
                ABombermanPlayer* Player = Cast<ABombermanPlayer>(StoredActorInfo->OwnerActor);
                if (Player)
                {
                    auto Context = StoredActorInfo->AbilitySystemComponent->MakeEffectContext();
                    Context.AddSourceObject(Player);

                    Player->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToTarget(
                        BombBackEffect, Player->GetAbilitySystemComponent(), 1, Context);

                    EndAbility(StoredHandle, StoredActorInfo, StoredActivationInfo, false, false);
                    return;
                }
            }
        }
    }



    AActor* BombObject = GetWorld()->SpawnActor<AActor>(BombActor, Location, Rotation);

    ABombermanBomb* Bomb = Cast<ABombermanBomb>(BombObject);
    if (Bomb)
    {
        ABombermanPlayer* Player = Cast<ABombermanPlayer>(StoredActorInfo->OwnerActor);
        if (Player)
        {
            Bomb->OriginalPlayer = Player;
        }

        Bomb->HasBeenSetUp = true;

        AMapGrid* Grid = Cast<AMapGrid>(GridActor);
        if (Grid)
            Grid->AddBomb(BombPoint);
    }

    EndAbility(StoredHandle, StoredActorInfo, StoredActivationInfo, false, false);
}



void UGA_PlantBomb::OnAnimNotifyBeginGP(FGameplayTag Tag, const FGameplayEventData* Payload)
{
    FName Name = Tag.GetTagName();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Tag.ToString());

    

    if (Name.Compare(FName("PlantBomb"))) 
        return;

    FVector Location = StoredActorInfo->OwnerActor->GetActorLocation();
    const FRotator Rotation = FRotator();


    AActor* GridActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMapGrid::StaticClass());
    FIntPoint BombPoint;


    if (GridActor)
    {
        AMapGrid* Grid = Cast<AMapGrid>(GridActor);
        if (Grid)
        {
            Location = Grid->GetClosestGridCenter(Location);
            BombPoint = Grid->GetClosestGridPoint(Location);

            if (Grid->IsBombOnPoint(BombPoint))
            {
                ABombermanPlayer* Player = Cast<ABombermanPlayer>(StoredActorInfo->OwnerActor);
                if (Player)
                {
                    auto Context = StoredActorInfo->AbilitySystemComponent->MakeEffectContext();
                    Context.AddSourceObject(Player);

                    Player->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToTarget(
                        BombBackEffect, Player->GetAbilitySystemComponent(), 1, Context);

                    EndAbility(StoredHandle, StoredActorInfo, StoredActivationInfo, false, false);
                    return;
                }
            }
        }
    }



    AActor* BombObject = GetWorld()->SpawnActor<AActor>(BombActor, Location, Rotation);

    ABombermanBomb* Bomb = Cast<ABombermanBomb>(BombObject);
    if (Bomb)
    {
        ABombermanPlayer* Player = Cast<ABombermanPlayer>(StoredActorInfo->OwnerActor);
        if (Player)
        {
            Bomb->OriginalPlayer = Player;
        }

        Bomb->HasBeenSetUp = true;

        AMapGrid* Grid = Cast<AMapGrid>(GridActor);
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

    float bombCount = ActorInfo->AbilitySystemComponent->GetGameplayAttributeValue(BombCountAttribute,bFound);


    return Super::CanActivateAbility(Handle,ActorInfo,SourceTags,TargetTags,OptionalRelevantTags) 
        && bFound && bombCount > 0;
}

void UGA_PlantBomb::FinishAbility(UAnimMontage* Montage, bool bInterrupted)
{
    EndAbility(StoredHandle, StoredActorInfo, StoredActivationInfo, false, false);
}


