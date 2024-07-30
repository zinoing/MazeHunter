// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze.h"
#include "Block.h"


AMaze::AMaze()
{
}

AMaze::AMaze(int MazeHeight, int MazeWidth) : HeightOfMaze(MazeHeight), WidthOfMaze(MazeWidth), NumOfBlocksSide(MazeHeight / 3)
{
    srand(time(0)); // Seed for random number generation
    Map = new int* [HeightOfMaze];
    // allocate Map
    for (int i = 0; i < HeightOfMaze; i++) {
        Map[i] = new int[WidthOfMaze]();
    }
    // allocate Blocks
    Blocks = new ABlock * [NumOfBlocksSide];
    for (int i = 0; i < NumOfBlocksSide; i++) {
        Blocks[i] = new ABlock[NumOfBlocksSide];
    }
}

void AMaze::Initialize()
{
    for (int i = 0; i < HeightOfMaze / 3; i++) {
        for (int j = 0; j < WidthOfMaze / 3; j++) {
            Blocks[i][j].SetCenter(3 * i + 1, 3 * j + 1);
            Blocks[i][j].Draw(Map);
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

void AMaze::CreatePattern()
{
    TArray<Coord> Coords;

    // Start from the Center block
    Coords.Push(Coord(NumOfBlocksSide / 2 + 1, NumOfBlocksSide / 2 + 1));

    while (Coords.Num() != 0) {
        Coord CoordToVisit = Coords[0];
        Coords.Pop();
        Blocks[CoordToVisit.Y][CoordToVisit.X].Visit();

        // Choose where to move
        TArray<int> directions = { 0, 1, 2, 3 };
        //random_shuffle(directions.begin(), directions.end());

        int Dy[4] = { 0, 0, -1, 1 };
        int Dx[4] = { -1, 1, 0, 0 };

        for (int i : directions) {
            int y = CoordToVisit.Y + Dy[i];
            int x = CoordToVisit.X + Dx[i];

            if (y >= NumOfBlocksSide || y < 0 || x >= NumOfBlocksSide || x < 0) {
                continue;
            }

            if (Blocks[y][x].IsVisited())
                continue;

            Blocks[y][x].DrawRoad(i, Map, HeightOfMaze, WidthOfMaze);
            Coords.Push(Coord(y, x));
        }
    }

    SetDestination(3);
}