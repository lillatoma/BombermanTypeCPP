// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "BombermanTypeCPP/GAS/GAS_AbilitySystemComponent.h"
#include "BombermanTypeCPP/GAS/GAS_GameplayAbility.h"
#include "BombermanTypeCPP/GAS/GAS_AttributeSet.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombermanFireDataHolder.generated.h"

UCLASS()
class BOMBERMANTYPECPP_API ABombermanFireDataHolder : public AActor
{
	GENERATED_BODY()
	
#pragma region GAS
public:
	// GAS declarations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
		class UGAS_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
		class UGAS_AttributeSet* Attributes;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const;


	virtual void InitializeAttributes();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	// Overload to initialize abilities for GAS, and component to store default abilities
	virtual void GiveAbilities();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TArray<TSubclassOf<class UGAS_GameplayAbility>> DefaultAbilities;
#pragma endregion
public:	
	// Sets default values for this actor's properties
	ABombermanFireDataHolder();

	FIntPoint CenterPoint;
	class AMapGrid* MapGrid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Function that finds the explosion's center
	virtual void CalculateGridPosition();
	//Function to find the grid object
	virtual class AMapGrid* FindGrid();
	//Function to spawn all objects for explosion 
	virtual void SpawnFireFull();
	///Function that spawns explosion on a single gridpoint
	///Returns true if spawned without obstacles
	virtual bool SpawnFire(FIntPoint point);

#pragma region Vars
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		TSubclassOf<class UGameplayAbility> DamageAbility;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		TSubclassOf<AActor> FireActor;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		FGameplayAttribute BombLengthAttribute;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ExplosionLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DestroyWaitTime;

#pragma endregion

private:

	void CallDestroy();

	bool hasBeenDestroyCalled = false;

public:	
	
	UFUNCTION()
		void InitiateDestroyCall();

#pragma region Damager

	TArray<class ABombermanPlayer*> AffectedPlayers;

	int AffectedCount = 0;

	void AddPlayerToAffected(class ABombermanPlayer* Player);

	void SetAffectedPlayerCount(int count);

#pragma endregion


};

