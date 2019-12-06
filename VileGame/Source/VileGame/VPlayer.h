// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VPawn.h"
#include "VPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class VILEGAME_API AVPlayer : public AVPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVPlayer();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* cameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* springArm;

	UPROPERTY(VisibleDefaultsOnly, Category = "Movement")
	float moveSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MovePlayer(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void MoveX(float value);

	void MoveY(float value);

	FVector2D movementDirection;

};
