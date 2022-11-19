// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_PlantBomb.h"
#include "BombermanTypeCPP\GAS\Gas_AbilitySystemComponent.h"
#include "BombermanTypeCPP\Game\BombermanBomb.h"
#include "BombermanTypeCPP\Game\BombermanPlayer.h"
#include "BombermanTypeCpp\Map\MapGrid.h"

#include "Kismet/GameplayStatics.h"

void UGA_PlantBomb::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    FVector Location = ActorInfo->OwnerActor->GetActorLocation();
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
        }
    }


    
    AActor* BombObject = GetWorld()->SpawnActor<AActor>(BombActor, Location, Rotation);

    ABombermanBomb* Bomb = Cast<ABombermanBomb>(BombObject);
    if (Bomb)
    {
        ABombermanPlayer* Player = Cast<ABombermanPlayer>(ActorInfo->OwnerActor);
        if (Player)
        {
            Bomb->OriginalPlayer = Player;
        }

        Bomb->HasBeenSetUp = true;

        AMapGrid* Grid = Cast<AMapGrid>(GridActor);
        if (Grid)
            Grid->AddBomb(BombPoint);
    }

    EndAbility(Handle, ActorInfo, ActivationInfo,false,false);
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