// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"

ABlock::ABlock() : Visited(false) {
    Center = Coord(-1, -1);
    PrimaryActorTick.bCanEverTick = false;
}

ABlock::ABlock(int Y, int X) : Visited(false) {
    Center.Y = Y;
    Center.X = X;
    PrimaryActorTick.bCanEverTick = false;
}

void ABlock::SetCenter(int Y, int X)
{
    Center.Y = Y;
    Center.X = X;
}

Coord ABlock::GetCenter()
{
    return Center;
}

bool ABlock::IsVisited()
{
    return Visited;
}

void ABlock::Visit()
{
    Visited = true;
}

void ABlock::DrawRoad(int Dir, TArray< TArray<int>>& Map, int MazeHeight, int MazeWidth)
{
    int Dy[4] = { 0, -1, 0, 1 };
    int Dx[4] = { -1, 0, 1, 0 };

    for (int i = 1; i <= 2; ++i) {
        int y = Center.Y + i * Dy[Dir];
        int x = Center.X + i * Dx[Dir];

        if (y >= MazeHeight / 3 * 3 - 1 || y <= 0 || x >= MazeWidth / 3 * 3 - 1 || x <= 0) {
            continue;
        }

        Map[y][x] = 0;
    }
}

void ABlock::Draw(TArray< TArray<int>>& Map)
{
    int y = Center.Y - 1;
    int x = Center.X - 1;

    for (int i = y; i < y + Rows; ++i) {
        for (int j = x; j < x + Cols; ++j) {
            Map[i][j] = Shape[i - y][j - x];
        }
    }
}