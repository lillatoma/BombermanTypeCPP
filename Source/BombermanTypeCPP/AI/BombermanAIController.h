// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BombermanAIController.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API ABombermanAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

	ABombermanAIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;

	virtual void OnPossess(APawn* InPawn) override;

	class ABombermanPlayer* Agent;

	UPROPERTY(transient)
		class UBehaviorTreeComponent* BTC;

	UPROPERTY(transient)
		class UBlackboardComponent* BBC;

	//Blackboard Key IDs
	uint8 EnemyKeyId;
	uint8 LocationKeyId;
	uint8 ContactKeyId;
};
