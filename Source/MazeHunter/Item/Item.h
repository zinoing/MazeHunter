// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8 {
	EIS_Initial UMETA(DisplayName = "Initial State"),
	EIS_Equipped UMETA(DisplayName = "Equipped State"),
	EIS_Dropped UMETA(DisplayName = "Dropped State"),

	EIS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class MAZEHUNTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere)
	EItemState ItemState;
};
