// Fill out your copyright notice in the Description page of Project Settings.


#include "VPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h" 

#include "Kismet/GameplayStatics.h"

// Sets default values
AVPlayer::AVPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
	springArm->SetRelativeRotation(FRotator(-30.0f, -90.0f, 0.0f));

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArm);

	movementDirection.Set(0.0f, 0.0f);

	moveSpeed = 500.0f;

}

// Called every frame
void AVPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlayer(DeltaTime);

}

void AVPlayer::MovePlayer(float DeltaTime)
{
	FVector position = GetActorLocation();

	movementDirection.Normalize();

	FVector p2 = position;

	//TODO come up with a better solution here
	//using this to move along walls
	position.Y += movementDirection.Y * moveSpeed * DeltaTime;
	if (!SetActorLocation(position, true))
	{
		position.Y = p2.Y;

	}
	position.X += movementDirection.X * moveSpeed * DeltaTime;
	if (!SetActorLocation(position, true))
	{
		position.X = p2.X;

	}

	movementDirection.Set(0.0f, 0.0f);
}

// Called to bind functionality to input
void AVPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveX", this, &AVPlayer::MoveX);
	InputComponent->BindAxis("MoveY", this, &AVPlayer::MoveY);


	InputComponent->BindAction("Enter", IE_Pressed, this, &AVPlayer::EnterPressed).bExecuteWhenPaused = true;
}


void AVPlayer::MoveX(float value)
{
	movementDirection.X = value;
}

void AVPlayer::MoveY(float value)
{
	movementDirection.Y = value;
}

void AVPlayer::EnterPressed()
{
	if (bIsPaused)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game unpaused"));
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bIsPaused = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game paused"));
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		bIsPaused = true;
	}
}

