// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VPickup.generated.h"

class USphereComponent;
class UMaterial;

UCLASS()
class VILEGAME_API AVPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* meshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* sphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	TArray<UMaterial*> materials;


	UPROPERTY(EditAnywhere, Category = "Time")
	float lifeTime = 5.0f;
	//move back to private
	UPROPERTY(EditAnywhere, Category = "test")
	int type;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Calling from blueprint until I can find out how to use OnDestroy in C++
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void Kill();

	int GetPoints();

	int GetType();

private:
	int points;


};
