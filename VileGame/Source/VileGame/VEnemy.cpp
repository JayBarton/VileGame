// Fill out your copyright notice in the Description page of Project Settings.


#include "VEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "VSpawnManager.h"
#include "VSpawner.h"

AVEnemy::AVEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVEnemy::BeginPlay()
{
	Super::BeginPlay();

	spawnManager = Cast<AVSpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVSpawnManager::StaticClass()));
	if (!spawnManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("No spawn manager found"));
		return;
	}

	score = 0;
	maxVelocity = 0.145f;
	wanderAngle = 0.0f;

	velocity.Set(0.0f, 0.0f);
}

void AVEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> occupiedSpawners;
	int size = spawnManager->spawners.Num();
	occupiedSpawners.Reserve(size);

	for (int i = 0; i < size; i++)
	{
		TArray<AActor*> child;
		//Not ideal...
		spawnManager->spawners[i]->GetAttachedActors(child);
		if (child.Num() > 0)
		{
			occupiedSpawners.Add(spawnManager->spawners[i]);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Number of Spawners: %i"), occupiedSpawners.Num());

	switch (state)
	{
	case EEnemyState::Wander:
		break;
	case EEnemyState::Seek:
		break;
	}
}
