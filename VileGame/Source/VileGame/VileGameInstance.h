// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VileGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VILEGAME_API UVileGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	int currentLevel = 1;
	
};
