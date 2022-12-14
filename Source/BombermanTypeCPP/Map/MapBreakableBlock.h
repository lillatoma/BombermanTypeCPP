// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapBreakableBlock.generated.h"

UCLASS()
class BOMBERMANTYPECPP_API AMapBreakableBlock : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Mesh;

#pragma region Vars

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prereqs")
		float SpawnPercentage;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
		TArray<TSubclassOf<AActor>> PowerupList;

#pragma endregion

public:	
	// Sets default values for this actor's properties
	AMapBreakableBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		void DecideSpawnPowerup();

};
