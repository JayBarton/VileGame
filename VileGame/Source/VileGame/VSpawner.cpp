// Fill out your copyright notice in the Description page of Project Settings.


#include "VSpawner.h"
#include "TimerManager.h" 
// Sets default values
AVSpawner::AVSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsFree = false;
	freeTime = 1.0f;
}

// Called when the game starts or when spawned
void AVSpawner::BeginPlay()
{
	Super::BeginPlay();

	SetUp();

}

// Called every frame
void AVSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVSpawner::SetUp()
{
	GetWorldTimerManager().SetTimer(TimerHandle_FreeTimer, this, &AVSpawner::SetFree, freeTime, false);

}

void AVSpawner::SetFree()
{
	bIsFree = true;
}

