// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

struct Coord {
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

    Coord Center;
    bool Visited;

public:
    ABlock();

    ABlock(int Y, int X);

    void SetCenter(int Y, int X);

    Coord GetCenter();

    bool IsVisited();

    void Visit();

    void DrawRoad(int Dir, int** Map, int MazeHeight, int MazeWidth);

    void Draw(int** Map);
};