// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

UCLASS()
class BOMBERMANTYPECPP_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FIntPoint Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float innerCircleRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float innerCircleBreakableChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float outerCircleRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float outerCircleBreakableChance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AMapGrid* MapGrid;

	virtual void GenerateMap();
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
	TSubclassOf<AActor> BreakableObject;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Prereqs")
	TSubclassOf<AActor> SolidObject;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GridDistance;
};
