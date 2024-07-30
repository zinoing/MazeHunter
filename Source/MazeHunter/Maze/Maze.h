// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maze.generated.h"

UCLASS()
class MAZEHUNTER_API AMaze : public AActor
{
	GENERATED_BODY()

private:
	int HeightOfMaze;
	int WidthOfMaze;
	int NumOfBlocksSide;

	int** Map;
	class ABlock** Blocks;

public:
	AMaze();

	AMaze(int MazeHeight, int MazeWidth);

	int** GetMap() { return Map; }

	void Initialize();

	void SetDestination(int Radius);

	void CreatePattern();

};