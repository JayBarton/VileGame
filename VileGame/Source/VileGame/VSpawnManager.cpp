// Fill out your copyright notice in the Description page of Project Settings.


#include "VSpawnManager.h"
#include "TimerManager.h" 
#include "VSpawner.h"
#include "VPickup.h"
#include "Kismet/GameplayStatics.h"


#include "Engine/World.h" 

// Sets default values
AVSpawnManager::AVSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	GetSpawners();

	GetWorldTimerManager().SetTimer(TimerHandle_StartTimer, this, &AVSpawnManager::StartGame, levelStartTime, false);
}

void AVSpawnManager::StartGame()
{
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnTimer, this, &AVSpawnManager::CheckSpawn, spawnTime, true, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TypeTimer, this, &AVSpawnManager::CheckActiveType, typeTime, true, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_LevelTimer, this, &AVSpawnManager::CheckLevelTime, levelTime, false);
}

void AVSpawnManager::GetSpawners()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVSpawner::StaticClass(), foundActors);
	spawners.Reserve(foundActors.Num());
	for (int i = 0; i < foundActors.Num(); i++)
	{
		spawners.Add(Cast<AVSpawner>(foundActors[i]));
	}
}

// Called every frame
void AVSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVSpawnManager::CheckSpawn()
{

	int randomSpawner = -1;
	TArray<AVSpawner*> freeSpawners;
	freeSpawners.Reserve(spawners.Num());

	//Could probably find a way to populate this fully on game start, since all spawners will be free at that point.
	for (int i = 0; i < spawners.Num(); i++)
	{
		if (spawners[i]->bIsFree)
		{
			freeSpawners.Add(spawners[i]);
		}
	}

	if (freeSpawners.Num() > 0)
	{
		randomSpawner = FMath::RandRange(0, freeSpawners.Num() - 1);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//Replace this with a socket
		FVector location = freeSpawners[randomSpawner]->GetActorLocation();
		location.Z += 60.0f;

		AVPickup* newPickup = GetWorld()->SpawnActor<AVPickup>(pickup, location, FRotator::ZeroRotator, SpawnParams);

		//If pickup fails to spawn or is immediately destroyed(if the player is standing on it when it spawns), stop here
		if (newPickup)
		{
			freeSpawners[randomSpawner]->bIsFree = false;
			newPickup->AttachToActor(freeSpawners[randomSpawner], FAttachmentTransformRules::KeepWorldTransform);
		}

	}


}

void AVSpawnManager::CheckActiveType()
{

	if (currentType == 0)
	{
		currentType = 1;
		UE_LOG(LogTemp, Warning, TEXT("Green!!!"));

	}
	else
	{
		currentType = 0;
		UE_LOG(LogTemp, Warning, TEXT("Red!!!"));

	}

}

void AVSpawnManager::CheckLevelTime()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnTimer);
	GetWorldTimerManager().ClearTimer(TimerHandle_TypeTimer);
	UE_LOG(LogTemp, Warning, TEXT("LEVEL DONE"));
}

