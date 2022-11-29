// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndingWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API UEndingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
		void SetWinloseText(bool won);

	UFUNCTION(BlueprintImplementableEvent)
		void ReloadScene();
};
