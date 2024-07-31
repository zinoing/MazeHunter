// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UCLASS()
class MAZEHUNTER_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeGenerator();

	UFUNCTION(BlueprintCallable, Category = "Maze")
	void Generate(int MazeHeight, int MazeWidth, int DestinationRadius);

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> Cube;
private:
	

};
