// Fill out your copyright notice in the Description page of Project Settings.


#include "VPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "VPickup.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#include "VResultsWidget.h" 
#include "Components/TextBlock.h" 



#include "Components/WidgetComponent.h" 

// Sets default values
AVPawn::AVPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	RootComponent = boxComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	pointDisplay = CreateDefaultSubobject<UWidgetComponent>(TEXT("pointDisplay"));
	pointDisplay->SetupAttachment(RootComponent);
	pointDisplay->SetWidgetClass(UVResultsWidget::StaticClass());
	pointDisplay->SetWidgetSpace(EWidgetSpace::Screen);
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
		int32 points = pickup->GetPoints();
		score += points;
		if (score < 0)
		{
			score = 0;
		}

		FString pointString = "";
		if (points > 0)
		{
			pointString += "+";
		}
		else
		{
			pointString += "-";
		}
		pointString += "5";
		
		if (auto tester = Cast<UVResultsWidget>(pointDisplay->GetUserWidgetObject()))
		{
			tester->results->SetText(FText::FromString(pointString));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("How you gonna play me like this"));
		}

		UE_LOG(LogTemp, Warning, TEXT("SCORE: %i"), score);

		UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);

		OtherActor->Destroy();
	}
}

