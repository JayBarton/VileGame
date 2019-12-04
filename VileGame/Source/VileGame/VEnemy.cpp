// Fill out your copyright notice in the Description page of Project Settings.

/*
Much of the enemy's behavior is based off of the steering behaviors of Craig W. Reynolds. http://www.red3d.com/cwr/steer/
*/

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
	{
		FVector2D avoidance(0.0f, 0.0f);
		
		FVector2D steer = Wander();

		if (FindClosestPickup(occupiedSpawners, steer))
		{
			avoidance = Avoid();

			state = EEnemyState::Seek;
		}

		steer += avoidance * 1.5f;
		velocity = Limit(velocity + steer, maxVelocity);

		FVector position = GetActorLocation();
		position.X += velocity.X * DeltaTime;
		position.Y += velocity.Y * DeltaTime;
		//Used to prevent the enemy from getting stuck on walls. If the enemy can't move forward, it reverses direction
		//May need to have some different reaction if the enemy bumps into the player, too easy for the player to "bully" the enemy as is.
		if (!SetActorLocation(position, true))
		{
			velocity *= -1;
			wanderAngle = 0.0f;
		}
		break;
	}
	case EEnemyState::Seek:
	{
		if (occupiedSpawners.Num() == 0)
		{
			wanderAngle = 0.0f;
			state = EEnemyState::Wander;
		}
		else
		{
			FVector2D toFollow;

			if (FindClosestPickup(occupiedSpawners, toFollow))
			{
				FVector2D position2D(toFollow.X, toFollow.Y);

				FVector2D steer2 = Seek(position2D);

				UE_LOG(LogTemp, Warning, TEXT("Seek: %s"), *steer2.ToString());

				FVector2D avoidance = Avoid();

				if (avoidance.Size() > 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("avoid: %s"), *avoidance.ToString());
					steer2 += avoidance * 1.5f;
					UE_LOG(LogTemp, Warning, TEXT("Seek + Avoid: %s"), *steer2.ToString());

				}



				velocity = Limit(velocity + steer2, maxVelocity);

				UE_LOG(LogTemp, Warning, TEXT("velocity: %s"), *velocity.ToString());


				FVector position2 = GetActorLocation();
				position2.X += velocity.X * DeltaTime;
				position2.Y += velocity.Y * DeltaTime;
				SetActorLocation(position2, true);
			}
			else
			{
				wanderAngle = 0.0f;
				state = EEnemyState::Wander;
			}
		}
		break;
	}
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

FVector2D AVEnemy::Wander()
{
	FVector2D circleCenter = velocity.GetSafeNormal() * 200;

	FVector2D displacement(0.0f, 1.0f);

	displacement *= 200;

	displacement = displacement.GetRotated(wanderAngle);

	wanderAngle += FMath::RandRange(0, 120) - 60;


	FVector2D wanderForce = circleCenter + displacement;

	FVector2D steer = Limit(wanderForce, maxForce);

	return steer;
}

FVector2D AVEnemy::Avoid()
{

	FVector2D avoidance(0.0f, 0.0f);

	FVector2D position(GetActorLocation().X, GetActorLocation().Y);

	FVector2D normalizedVelocity = velocity.GetSafeNormal();

	FVector2D ahead1 = position + normalizedVelocity * 128;
	FVector2D ahead2 = position + normalizedVelocity * 64;

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
			if (Intersect(ahead1, ahead2, currentPosition, 64) && 
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
		
		//avoidance = (ahead1 - closestPosition).GetSafeNormal() * 25.0f;
		avoidance.Normalize();
		avoidance *= 25.0f;

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
