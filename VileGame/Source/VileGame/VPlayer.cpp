// Fill out your copyright notice in the Description page of Project Settings.


#include "VPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h" 

// Sets default values
AVPlayer::AVPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	RootComponent = meshComp;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
	springArm->SetRelativeRotation(FRotator(-30.0f, -90.0f, 0.0f));

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArm);

	movementDirection.Set(0.0f, 0.0f);

	moveSpeed = 500.0f;

}

// Called when the game starts or when spawned
void AVPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector position = GetActorLocation();

	movementDirection.Normalize();

	position.X += movementDirection.X * moveSpeed * DeltaTime;
	position.Y += movementDirection.Y * moveSpeed * DeltaTime;

	UE_LOG(LogTemp, Warning, TEXT("%f, %f"), movementDirection.X, movementDirection.Y);

	SetActorLocation(position);

	movementDirection.Set(0.0f, 0.0f);


}

// Called to bind functionality to input
void AVPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveX", this, &AVPlayer::MoveX);
	InputComponent->BindAxis("MoveY", this, &AVPlayer::MoveY);

}

void AVPlayer::MoveX(float value)
{
	movementDirection.X = value;
}

void AVPlayer::MoveY(float value)
{
	movementDirection.Y = value;
}

