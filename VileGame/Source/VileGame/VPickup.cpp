// Fill out your copyright notice in the Description page of Project Settings.


#include "VPickup.h"
#include "Components/SphereComponent.h" 
#include "VSpawnManager.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AVPickup::AVPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	RootComponent = sphereComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->CastShadow = 0;

	materials.SetNum(2);

}

// Called when the game starts or when spawned
void AVPickup::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(lifeTime);

	type = FMath::RandRange(0, 1);
	meshComp->SetMaterial(0, materials[type]);

	Cast<AVSpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVSpawnManager::StaticClass()))->GameOver.AddDynamic(this, &AVPickup::EndIt);


}

// Called every frame
void AVPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AVPickup::Kill()
{
	//UE_LOG(LogTemp, Warning, TEXT("GONE!"));
}

void AVPickup::EndIt()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroying pickups!"));
	Destroy();
}

int AVPickup::GetPoints()
{
	//EWWW!
	//Should probably error check this...
	int activeType = Cast<AVSpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVSpawnManager::StaticClass()))->currentType;
	if (type == activeType)
	{
		return 5;
	}
	return -5;
}

int AVPickup::GetType()
{
	return type;
}

