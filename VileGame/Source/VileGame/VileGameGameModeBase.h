// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VileGameGameModeBase.generated.h"

/**
 * 
 */

class UVResultsWidget;
UCLASS()
class VILEGAME_API AVileGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void CompleteLevel();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<AActor> displayObject;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UVResultsWidget> resultsScreen;

	

private:
	void EnterPressed();

	int32 currentLevel;

	int32 lastLevel = 2;

	bool bIsGameOver = false;
};
