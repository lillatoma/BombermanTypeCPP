// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanAIController.h"
#include "SmartObject.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BombermanTypeCPP/Game/BombermanPlayer.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

ABombermanAIController::ABombermanAIController(const FObjectInitializer& ObjectInitializer)
{
    BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
    BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
}

void ABombermanAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ABombermanPlayer* Chr = Cast<ABombermanPlayer>(InPawn);

    if (Chr && Chr->TreeAsset)
    {
        BBC->InitializeBlackboard(*Chr->TreeAsset->BlackboardAsset);

        EnemyKeyId = BBC->GetKeyID("TargetActor");
        LocationKeyId = BBC->GetKeyID("MoveToLocation");
        ContactKeyId = BBC->GetKeyID("Contact");

        BTC->StartTree(*Chr->TreeAsset);
    }
}


void ABombermanAIController::BeginPlay()
{
    Super::BeginPlay();

    RunBehaviorTree(BehaviorTree);

    if (!Agent)
    {
        ABombermanPlayer* Chr = Cast<ABombermanPlayer>(GetPawn());
        if (!Chr)
            return;
        Agent = Chr;
    }

    if (Agent->SmartObject)
    {
        FGameplayTag SubTag;
        BTC->SetDynamicSubtree(SubTag, Agent->SmartObject->SubTree);
    }

}