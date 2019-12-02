// Fill out your copyright notice in the Description page of Project Settings.


#include "VileGameGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "VileGameInstance.h"

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

	UVileGameInstance* GI = Cast<UVileGameInstance>(GetGameInstance());
	if (GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Level : %i"), GI->currentLevel);
	}

}

void AVileGameGameModeBase::CompleteLevel()
{
	//determine winner

	UE_LOG(LogTemp, Warning, TEXT("LEVEL DONE"));


	UVileGameInstance* GI = Cast<UVileGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->currentLevel++;
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent->BindAction("Enter", IE_Pressed, this, &AVileGameGameModeBase::EnterPressed);

}

void AVileGameGameModeBase::EnterPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Map2"));
}
