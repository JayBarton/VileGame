// Fill out your copyright notice in the Description page of Project Settings.


#include "VEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "VSpawnManager.h"
#include "VSpawner.h"
#include "VPickup.h"


AVEnemy::AVEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	maxVelocity = 500.0f;

}

void AVEnemy::BeginPlay()
{
	Super::BeginPlay();

	spawnManager = Cast<AVSpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVSpawnManager::StaticClass()));
	if (!spawnManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("No spawn manager found"));
		return;
	}

	score = 0;
	wanderAngle = 0.0f;

	velocity.Set(0.0f, 0.0f);
}

void AVEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> occupiedSpawners;
	int size = spawnManager->spawners.Num();
	occupiedSpawners.Reserve(size);

	for (int i = 0; i < size; i++)
	{
		TArray<AActor*> child;
		//Not ideal...
		spawnManager->spawners[i]->GetAttachedActors(child);
		if (child.Num() > 0)
		{
			occupiedSpawners.Add(spawnManager->spawners[i]);
		}
	}

	switch (state)
	{
	case EEnemyState::Wander:
		break;
	case EEnemyState::Seek:

	/*	if (occupiedSpawners.Num() == 0)
		{
			wanderAngle = 0.0f;
			state = EEnemyState::Wander;
		}*/

		FVector2D toFollow;

		if (FindClosestPickup(occupiedSpawners, toFollow))
		{
			FVector2D position2D(toFollow.X, toFollow.Y);

			FVector2D steer = Seek(position2D);

			steer += Avoid() * 1.5f;

			velocity = Limit(velocity + steer, maxVelocity);

			FVector position = GetActorLocation();

			position.X += velocity.X * DeltaTime;
			position.Y += velocity.Y * DeltaTime;
			SetActorLocation(position, true);
		}
	/*	else
		{
			wanderAngle = 0.0f;
			state = EEnemyState::Wander;
		}*/

		break;
	}
}

FVector2D AVEnemy::Seek(FVector2D toFollow)
{

	FVector2D position(GetActorLocation().X, GetActorLocation().Y);
	FVector2D direction = (toFollow - position);
	direction.Normalize();

	FVector2D desired = direction * maxVelocity;

	FVector2D steer = desired - velocity;

	steer = Limit(steer, maxForce);

	return steer;
}

FVector2D AVEnemy::Limit(FVector2D vec, float max)
{
	if (vec.Size() > max)
	{
		vec.Normalize();
		return vec * max;
	}
	return vec;
}

FVector2D AVEnemy::Avoid()
{

	FVector2D avoidance(0.0f, 0.0f);

	FVector2D position(GetActorLocation().X, GetActorLocation().Y);

	FVector2D normalizedVelocity = velocity.GetSafeNormal();

	FVector2D ahead1 = position + normalizedVelocity * 4;
	FVector2D ahead2 = position + normalizedVelocity * 2;

	AActor* closest = nullptr;

	for (int i = 0; i < spawnManager->spawners.Num(); i++)
	{
		TArray<AActor*> child;
		//Not ideal...
		AVSpawner* current = spawnManager->spawners[i];
		current->GetAttachedActors(child);
		if (child.Num() > 0 && Cast<AVPickup>(child[0])->GetType() != spawnManager->currentType)
		{
			FVector2D currentPosition(current->GetActorLocation().X, current->GetActorLocation().Y);
			if (Intersect(ahead1, ahead2, currentPosition, 2) && 
				(!closest || (position - currentPosition).Size() < 
				(GetActorLocation() - closest->GetActorLocation()).Size()))
			{
				closest = current;
			}
		}
	}
	if (closest)
	{
		FVector2D closestPosition(closest->GetActorLocation().X, closest->GetActorLocation().Y);
		
		avoidance = (ahead1 - closestPosition).GetSafeNormal() * 0.25f;
	}
	return avoidance;
}

bool AVEnemy::Intersect(FVector2D ahead1, FVector2D ahead2, FVector2D toAvoid, float r)
{
	return (ahead1 - toAvoid).Size() <= r || (ahead2 - toAvoid).Size() <= r;
}

bool AVEnemy::FindClosestPickup(TArray<AActor*> occupied, FVector2D& toFollow)
{
	float nearest = -1.0f;

	for (int i = 0; i < occupied.Num(); i++)
	{
		TArray<AActor*> child;
		//Not ideal...
		occupied[i]->GetAttachedActors(child);
		if (Cast<AVPickup>(child[0])->GetType() == spawnManager->currentType)
		{
			FVector spawnerPosition = occupied[i]->GetActorLocation();
			float distance = (GetActorLocation() - spawnerPosition).Size();

			if (nearest < 0 || distance < nearest)
			{
				nearest = distance;
				toFollow = FVector2D(spawnerPosition.X, spawnerPosition.Y);
			}
		}
	}

	return nearest >= 0;
}
