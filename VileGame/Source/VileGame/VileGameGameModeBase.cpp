// Fill out your copyright notice in the Description page of Project Settings.


#include "VileGameGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "VileGameInstance.h"

#include "VPlayer.h"
#include "VEnemy.h"

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

	//Just testing for now...
	UVileGameInstance* GI = Cast<UVileGameInstance>(GetGameInstance());
	if (GI)
	{
		FString levelName = "Map" + FString::FromInt(GI->currentLevel);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *levelName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO game instance found"));
	}

}

void AVileGameGameModeBase::CompleteLevel()
{
	//determine winner
	int playerScore = Cast<AVPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AVPlayer::StaticClass()))->score;
	int enemyScore = Cast<AVEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), AVEnemy::StaticClass()))->score;


	UVileGameInstance* GI = Cast<UVileGameInstance>(GetGameInstance());
	if (GI)
	{
		if (playerScore > enemyScore)
		{
			UE_LOG(LogTemp, Warning, TEXT("PLAYER WINS"));

			GI->currentLevel++;

			if (GI->currentLevel > lastLevel)
			{
				//return to title
				UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
				GI->currentLevel = 1;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PLAYER LOSES"));
		}
		currentLevel = GI->currentLevel;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO game instance found"));
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent->BindAction("Enter", IE_Pressed, this, &AVileGameGameModeBase::EnterPressed);

}

void AVileGameGameModeBase::EnterPressed()
{
	//will need some check to return to the title.
	FString levelName = "Map" + FString::FromInt(currentLevel);

	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}
