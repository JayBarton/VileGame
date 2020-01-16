// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VileGameGameModeBase.generated.h"

/**
 * 
 */

class UVResultsWidget;
class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseDelegate);

UCLASS()
class VILEGAME_API AVileGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void CompleteLevel();

	UFUNCTION(BlueprintCallable)
	void PauseGame();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FPauseDelegate GamePaused;


protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<AActor> displayObject;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UVResultsWidget> resultsScreen;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> pauseScreen;

	UUserWidget* pauseWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* WinSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* LoseSound = nullptr;
	

private:

	void EnterPressed();

	UInputComponent* inputComponent;

	int32 currentLevel;

	int32 lastLevel = 1;

	int32 pauseKey;

	bool bIsGameOver = false;

	bool bIsPaused = false;
};
