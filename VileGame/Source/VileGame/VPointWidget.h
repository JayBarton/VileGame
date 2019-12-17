// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VPointWidget.generated.h"

/**
 * 
 */
UCLASS()
class VILEGAME_API UVPointWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
		FString pointValue;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void DisplayPoints(const FString& points);
};
