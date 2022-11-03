// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGridpoint.h"
#include "GameFramework/Actor.h"

UMapGridpoint::UMapGridpoint()
{

}

UMapGridpoint::UMapGridpoint(FIntPoint coord, EMGPMapGridpointType type, AActor* block)
{
	Coord = coord;
	Type = type;
	Block = block;
}

