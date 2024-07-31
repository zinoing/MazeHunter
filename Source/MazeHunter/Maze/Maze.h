// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maze.generated.h"

class ABlock;

UCLASS()
class MAZEHUNTER_API AMaze : public AActor
{
	GENERATED_BODY()

private:
	int HeightOfMaze;
	int WidthOfMaze;
	int NumOfBlocksSide;

	TArray< TArray<int>> Map;
	TArray<TArray<ABlock*>> Blocks;

public:
	AMaze();
	~AMaze();

	TArray< TArray<int>> GetMap() { return Map; }

	void Initialize(int MazeHeight, int MazeWidth);

	void SetDestination(int Radius);

	void CreatePattern(int DestinationRadius);

};