// Fill out your copyright notice in the Description page of Project Settings.


#include "VileGameGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "VileGameInstance.h"

#include "VPlayer.h"
#include "VEnemy.h"

#include "VResultsWidget.h" 
#include "Components/TextBlock.h" 



#include "GameFramework/InputSettings.h" 

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

	inputComponent = UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent;
	inputComponent->BindAction("Enter", IE_Pressed, this, &AVileGameGameModeBase::PauseGame).bExecuteWhenPaused = true;
	//Only easy way I can figure out how to unbind the pause
	pauseKey = inputComponent->GetNumActionBindings() - 1;

}

void AVileGameGameModeBase::CompleteLevel()
{
	//determine winner
	int playerScore = Cast<AVPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AVPlayer::StaticClass()))->score;
	int enemyScore = Cast<AVEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), AVEnemy::StaticClass()))->score;

	UVResultsWidget* resultsWidget = CreateWidget<UVResultsWidget>(GetWorld(), resultsScreen);
	resultsWidget->AddToViewport();

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
				resultsWidget->results->SetText(FText::FromString("GAME OVER"));

				GI->currentLevel = 1;
				bIsGameOver = true;
			}
			else
			{
				resultsWidget->results->SetText(FText::FromString("PLAYER WINS"));

			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PLAYER LOSES"));
			resultsWidget->results->SetText(FText::FromString("PLAYER LOSES"));

		}
		currentLevel = GI->currentLevel;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO game instance found"));
	}

	inputComponent->RemoveActionBinding(pauseKey);
	inputComponent->BindAction("Enter", IE_Pressed, this, &AVileGameGameModeBase::EnterPressed);

}

void AVileGameGameModeBase::PauseGame()
{

	if (bIsPaused)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game unpaused"));
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bIsPaused = false;
		pauseWidget->RemoveFromParent();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game paused"));
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		bIsPaused = true;

		pauseWidget = CreateWidget<UUserWidget>(GetWorld(), pauseScreen);
		pauseWidget->AddToViewport();
	}
}

void AVileGameGameModeBase::EnterPressed()
{
	FString levelName;
	if (bIsGameOver)
	{
		levelName = "TitleScreen";
	}
	else
	{
		levelName = "Map" + FString::FromInt(currentLevel);
	}
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}
