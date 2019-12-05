// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VResultsWidget.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class VILEGAME_API UVResultsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* results;
	
};
