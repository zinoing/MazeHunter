// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

struct Coordinate {
    int Y;
    int X;
};


UCLASS()
class MAZEHUNTER_API ABlock : public AActor
{
	GENERATED_BODY()
	
private:
    static const int Rows = 3;
    static const int Cols = 3;

    int Shape[Rows][Cols] = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    };

    Coordinate Center;
    bool Visited;

public:
    ABlock();

    ABlock(int Y, int X);

    void SetCenter(int Y, int X);

    Coordinate GetCenter();

    bool IsVisited();

    void Visit();

    void DrawRoad(int Dir, TArray< TArray<int>>& Map, int MazeHeight, int MazeWidth);

    void Draw(TArray< TArray<int>>& Map);
};