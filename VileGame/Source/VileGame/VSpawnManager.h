// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VSpawnManager.generated.h"



/*
	public GameObject[] spawners;
	public GameObject pickup;

	public GameObject displayObject;

	public GameObject pauseMenu;

	public PlayerController pc;
	public EnemyController ec;

	public Material [] materials;

	public Text timeText;
	public Text promptText;
	public Text startText;

	float spawnTime = 0.75f; //make random
	float spawnTimer = 0.0f;
	float typeTime = 5.0f; //should also be random
	float typetimer = 0.0f;

	float levelTime = 20.0f;
	float startTime = 5.0f;

	public int currentType;

	enum State {START_GAME, PLAYING, END_GAME, PAUSE};
	State state = State.START_GAME;

	//pause options
	const int CONTINUE = 0;
	const int QUIT = 1;

	const int NUMBER_OF_LEVELS = 3;
	public static int currentLevel = 1;
*/

class AVSpawner;
class AVPickup;

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

	FTimerHandle TimerHandle_LevelTimer;

	TArray<AVSpawner*> spawners;

	//UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	//TSubclassOf<AVSpawner> spawnerType;

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

//	UPROPERTY(VisibleAnywhere)
	int currentType = 0;

};
