// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapSolidBlock.generated.h"

UCLASS()
class BOMBERMANTYPECPP_API AMapSolidBlock : public AActor
{
	GENERATED_BODY()
	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Mesh;


public:	
	// Sets default values for this actor's properties
	AMapSolidBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
