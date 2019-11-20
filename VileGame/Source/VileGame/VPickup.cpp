// Fill out your copyright notice in the Description page of Project Settings.


#include "VPickup.h"
#include "Components/SphereComponent.h" 

// Sets default values
AVPickup::AVPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	RootComponent = sphereComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVPickup::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(lifeTime);
}

// Called every frame
void AVPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AVPickup::Kill()
{
	UE_LOG(LogTemp, Warning, TEXT("GONE!"));
}

int AVPickup::GetPoints()
{
	return 0;
}

int AVPickup::GetType()
{
	return 0;
}

