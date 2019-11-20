// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VSpawner.generated.h"

UCLASS()
class VILEGAME_API AVSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bIsFree;
	bool isFreeing;

	float freeTime;

	FTimerHandle TimerHandle_FreeTimer;

	UFUNCTION()
	void SetUp();

	void SetFree();

	//TODO maybe include some sort of function to stop the timer if the game has ended, we'll see what that looks like

};
