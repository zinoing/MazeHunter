// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"
#include "Maze.h"

// Sets default values
AMazeGenerator::AMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMazeGenerator::Generate(int MazeHeight, int MazeWidth)
{
	AMaze Maze(MazeHeight, MazeWidth);
	Maze.Initialize();
	Maze.CreatePattern();

	FActorSpawnParameters SpawnParams;
	FVector Loc;
	FRotator Rot;

	UWorld* World = GetWorld();
	int** Map = Maze.GetMap();

	for (int i = 0; i < MazeHeight; i++) {
		for (int j = 0; j < MazeWidth; j++) {
			if (Map[i][j] == 0) continue;

			World->SpawnActor<AActor>(Cube, FVector(i, j, 0.0f), Rot, SpawnParams);
		}
	}
}

