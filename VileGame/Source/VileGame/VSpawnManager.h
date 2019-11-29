// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VSpawnManager.generated.h"


class AVSpawner;
class AVPickup;

class UMaterial;


UCLASS()
class VILEGAME_API AVSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartGame();

	void GetSpawners();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CheckSpawn();

	UFUNCTION()
	void CheckActiveType();

	UFUNCTION()
	void CheckLevelTime();

	FTimerHandle TimerHandle_SpawnTimer;

	FTimerHandle TimerHandle_TypeTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	FTimerHandle TimerHandle_LevelTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	FTimerHandle TimerHandle_StartTimer;

	TArray<AVSpawner*> spawners;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AVPickup> pickup;

	//Time between spawns
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float spawnTime = 0.75f; //make random?
	//Time between changing the type
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float typeTime = 5.0f; //make random?
	//Length of the level
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float levelTime = 20.0f;
	//Delay before level start
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float levelStartTime = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Type")
	int currentType = 0;


	//Find a better way of handling this, it's duplicated from VPickup
	UPROPERTY(EditDefaultsOnly, Category = "Material")
	TArray<UMaterial*> materials;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<AActor> displayObject;

	UStaticMeshComponent* displayMesh = nullptr;

};
