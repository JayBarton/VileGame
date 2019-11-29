// Fill out your copyright notice in the Description page of Project Settings.


#include "VileGameGameModeBase.h"

void AVileGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector location(0.0f, 0.0f, 800.0f);

	if (!GetWorld()->SpawnActor<AActor>(displayObject, location, FRotator::ZeroRotator, SpawnParams))
	{
		UE_LOG(LogTemp, Warning, TEXT("No Display Object"));
	}


}
