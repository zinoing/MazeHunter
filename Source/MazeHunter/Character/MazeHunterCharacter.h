// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MazeHunterCharacter.generated.h"

UCLASS()
class MAZEHUNTER_API AMazeHunterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMazeHunterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FORCEINLINE void SetOverlappingItem(AItem* Item);
	UFUNCTION()
	bool IsItemEquipped();
protected:
	void MoveForward(float Value);
	void EquipItem();
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnRep_OverlappingItem(AItem* LastItem);

	UFUNCTION(Server, Reliable)
	void ServerEquipItem();

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingItem)
	class AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere)
	class UOnHandComponent* OnHand;
};
