// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "BombermanTypeCPP/GAS/GAS_AbilitySystemComponent.h"
#include "BombermanTypeCPP/GAS/GAS_GameplayAbility.h"
#include "BombermanTypeCPP/GAS/GAS_AttributeSet.h"


#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "BombermanPlayer.generated.h"



UCLASS()
class BOMBERMANTYPECPP_API ABombermanPlayer : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()






public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UCameraComponent* Camera;

public:
	// Sets default values for this pawn's properties
	ABombermanPlayer();
	
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveForce = 500.f;

	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;

	class AMapGrid* Grid;
	class AMapGrid* GetGrid();
	void UpdateGridPosition();
	int GridCharacterIndex = -1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int GetCharacterIndex();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* TreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		class ASmartObject* SmartObject;


private:
	void MoveRight(float value);
	void MoveForward(float value);

	//void PlayPlantAnim();

	//UFUNCTION()
	//void OnAnimNotifyBegin(FName Name, const FBranchingPointNotifyPayload& Payload);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		FGameplayAttribute BombCountAttribute;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		TSubclassOf<UGameplayAbility> BombPlantAbility;

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
	//	class UAnimMontage* PlantMontage;

};
