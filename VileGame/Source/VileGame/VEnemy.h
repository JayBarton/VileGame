// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VPawn.h"
#include "VEnemy.generated.h"

enum class EEnemyState : uint8
{
	Wander,
	Seek
};

class AVSpawnManager;
UCLASS()
class VILEGAME_API AVEnemy : public AVPawn
{
	GENERATED_BODY()

public:
	AVEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:

	virtual void Tick(float DeltaTime) override;

	EEnemyState state = EEnemyState::Wander;

	AVSpawnManager* spawnManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "Test")
	float maxVelocity;
	UPROPERTY(EditAnywhere, Category = "Test")
	float maxForce = 0.01125f;
	UPROPERTY(EditAnywhere, Category = "Test")
	float wanderAngle;

private:
	FVector2D velocity;
};
