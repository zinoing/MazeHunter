// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OnHandComponent.generated.h"

class AItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAZEHUNTER_API UOnHandComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	friend class AMazeHunterCharacter;

	UOnHandComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void EquipItem(AItem* OverlappingItem);

protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming) { bAiming = bIsAiming; }
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	 AMazeHunterCharacter* Character;

	UPROPERTY(Replicated)
	AItem* EquippedItem;

	UPROPERTY(Replicated)
	bool bAiming;
};
