// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "BombermanTypeCPP/GAS/GAS_AbilitySystemComponent.h"
#include "BombermanTypeCPP/GAS/GAS_GameplayAbility.h"
#include "BombermanTypeCPP/GAS/GAS_AttributeSet.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombermanBomb.generated.h"

UCLASS()
class BOMBERMANTYPECPP_API ABombermanBomb : public AActor
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
	ABombermanBomb();

#pragma region Vars

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FuseTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasBeenSetUp = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		 class ABombermanPlayer* OriginalPlayer;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		TSubclassOf<class UGameplayAbility> ExplosionAbility;

#pragma endregion

public:
	virtual void Explode();


private:
	UFUNCTION()
		void DestroySelf();

	void CallDestroy();

	bool hasBeenDetonated = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
