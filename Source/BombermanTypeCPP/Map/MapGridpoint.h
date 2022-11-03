// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapGridpoint.generated.h"

UENUM(BlueprintType)
enum class EMGPMapGridpointType : uint8
{
	Air,
	Breakable,
	Solid
};

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UMapGridpoint : public UObject
{
	GENERATED_BODY()
public:
	UMapGridpoint();
	UMapGridpoint(FIntPoint Coord, EMGPMapGridpointType Type = EMGPMapGridpointType::Air, class AActor* Block = nullptr);
	//~MapGridpoint();

	
	FIntPoint Coord;
	EMGPMapGridpointType Type = EMGPMapGridpointType::Air;
	class AActor* Block = nullptr;

};
