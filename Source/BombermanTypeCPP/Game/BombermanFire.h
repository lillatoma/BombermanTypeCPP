// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombermanFire.generated.h"

UCLASS()
class BOMBERMANTYPECPP_API ABombermanFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABombermanFire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ABombermanFireDataHolder* FireData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	

	void CallDestroy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ExplosionTime = 0.25f;
};
