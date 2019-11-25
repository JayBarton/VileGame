// Fill out your copyright notice in the Description page of Project Settings.


#include "VPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "VPickup.h"

// Sets default values
AVPawn::AVPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	RootComponent = boxComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVPawn::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AVPawn::OnOverlapBegin);

}


void AVPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AVPickup * pickup = Cast<AVPickup>(OtherActor))
	{
		score += pickup->GetPoints();
		if (score < 0)
		{
			score = 0;
		}

		UE_LOG(LogTemp, Warning, TEXT("SCORE: %i"), score);

		OtherActor->Destroy();
	}
}

