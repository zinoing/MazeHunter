// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"
#include "Maze.h"

// Sets default values
AMazeGenerator::AMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMazeGenerator::Generate(int MazeHeight, int MazeWidth, int DestinationRadius)
{
	if (UWorld* World = GetWorld()) // Check if World is valid
	{
		// Define spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// Spawn the actor
		AMaze* Maze = World->SpawnActor<AMaze>(AMaze::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (Maze)
		{
			// Initialize maze parameters
			Maze->Initialize(MazeHeight, MazeWidth);
			Maze->CreatePattern(DestinationRadius);

			FRotator Rot;

			TArray< TArray<int>> Map = Maze->GetMap();

			for (int i = 0; i < MazeHeight; i++) {
				for (int j = 0; j < MazeWidth; j++) {
					
					if (Map[i][j] == 0) continue;
					if (Map[i][j] == 2) continue;

					float lengthOfCube = 100.0f;
					World->SpawnActor<AActor>(Cube, FVector(-5000 + i * lengthOfCube, -5000 + j * lengthOfCube, lengthOfCube / 2), Rot, SpawnParams);
				}
			}
		}
	}
}

