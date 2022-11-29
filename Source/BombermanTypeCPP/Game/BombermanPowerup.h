// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombermanPowerup.generated.h"

UCLASS()
class BOMBERMANTYPECPP_API ABombermanPowerup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABombermanPowerup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddOnGrid();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prereqs")
		TSubclassOf< class UGameplayEffect > PowerupEffect;

private:
	class AMapGrid* Grid;

	class AMapGrid* GetGrid();
};
