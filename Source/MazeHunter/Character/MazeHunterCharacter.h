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
	UFUNCTION()
	bool IsAiming();

	float GetBaseWalkSpeed() { return BaseWalkSpeed; }
	float GetAimWalkSpeed() { return AimWalkSpeed; }
protected:
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void AimOffset(float DeltaTime);

	void MoveForward(float Value);
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

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

public:
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE AItem* GetEquippedItem() const;
};
