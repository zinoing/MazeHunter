// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze.h"
#include "Block.h"


AMaze::AMaze()
{
}

AMaze::~AMaze()
{
}

void AMaze::Initialize(int MazeHeight, int MazeWidth)
{
    HeightOfMaze = MazeHeight;
    WidthOfMaze = MazeWidth;
    NumOfBlocksSide = MazeHeight / 3;

    srand(time(0)); // Seed for random number generation

    for (int i = 0; i < HeightOfMaze; i++) {
        TArray<int> PartOfMap;
        PartOfMap.Init(0, MazeWidth);
        Map.Add(PartOfMap);

        // TArray<ABlock*> Block;
        // Block.Init(NewObject<ABlock>(), NumOfBlocksSide);
        // Blocks.Add(Block);
    
        // Initialize Blocks
        TArray<ABlock*> Block;
        Block.SetNum(NumOfBlocksSide); // Set size of the array

        // Create individual ABlock instances
        for (int j = 0; j < NumOfBlocksSide; j++) {
            ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(); // Create a new ABlock object
            Block[j] = NewBlock;
        }

        Blocks.Add(Block);
    }

    for (int i = 0; i < HeightOfMaze / 3; i++) {
        for (int j = 0; j < WidthOfMaze / 3; j++) {
            Blocks[i][j]->SetCenter(3 * i + 1, 3 * j + 1);
            Blocks[i][j]->Draw(Map);
        }
    }
}

void AMaze::SetDestination(int Radius)
{
    int CenterY = HeightOfMaze / 2 - 1;
    int CenterX = WidthOfMaze / 2 - 1;
    for (int y = 0; y < HeightOfMaze; ++y) {
        for (int x = 0; x < WidthOfMaze; ++x) {
            // Calculate the distance from (x, y) to the Center (CenterX, CenterY)
            int DistanceOfX = x - CenterX;
            int DistanceOfY = y - CenterY;
            if (sqrt(DistanceOfX * DistanceOfX + DistanceOfY * DistanceOfY) <= Radius) {
                Map[y][x] = 2;  // Set the pixel inside the circle to 0
            }
        }
    }
}

void AMaze::CreatePattern(int DestinationRadius)
{
    TArray<Coord> Coords;

    // Start from the Center block
    Coords.Add(Coord(NumOfBlocksSide / 2 + 1, NumOfBlocksSide / 2 + 1));

    for (int i = 0; i < NumOfBlocksSide; ++i) {
        for (int j = 0; j < NumOfBlocksSide; ++j) {

            if (Blocks[i][j]->IsVisited())
                continue;

            Coords.Add(Coord(i, j));

            while (Coords.Num() != 0) {
                Coord CoordToVisit = Coords[0];
                Coords.RemoveAt(0);
                Blocks[CoordToVisit.Y][CoordToVisit.X]->Visit();

                // Choose where to move
                TArray<int> Directions = { 0, 1, 2, 3 };

                // shuffle
                const int32 LastIndex = Directions.Num() - 1;
                for (int32 k = 0; k < LastIndex; ++k)
                {
                    int32 Index = FMath::RandRange(0, LastIndex);
                    if (k != Index)
                    {
                        Directions.Swap(k, Index);
                    }
                }

                int Dy[4] = { 0, -1, 0, 1 };
                int Dx[4] = { -1, 0, 1, 0 };

                for (int dir : Directions) {
                    int y = CoordToVisit.Y + Dy[dir];
                    int x = CoordToVisit.X + Dx[dir];

                    if (y >= NumOfBlocksSide || y < 0 || x >= NumOfBlocksSide || x < 0) {
                        continue;
                    }

                    if (Blocks[y][x]->IsVisited()) continue;
                    
                    Blocks[CoordToVisit.Y][CoordToVisit.X]->DrawRoad(dir, Map, HeightOfMaze, WidthOfMaze);
                    Coords.Add(Coord(y, x));
                    break;
                }
            }
        }
    }

    SetDestination(DestinationRadius);
}