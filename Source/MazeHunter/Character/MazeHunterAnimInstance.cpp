// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeHunterAnimInstance.h"
#include "MazeHunterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMazeHunterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MazeHunterCharacter = Cast<AMazeHunterCharacter>(TryGetPawnOwner());
}

void UMazeHunterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MazeHunterCharacter == nullptr) {
		MazeHunterCharacter = Cast<AMazeHunterCharacter>(TryGetPawnOwner());
	}

	if (MazeHunterCharacter == nullptr) return;

	FVector Velocity = MazeHunterCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	bIsInAir = MazeHunterCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = MazeHunterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bItemEquipped = MazeHunterCharacter->IsItemEquipped();
	bIsCrouched = MazeHunterCharacter->bIsCrouched;
	bAiming = MazeHunterCharacter->IsAiming();

	// Offsets Yaw for Strafing
	FRotator AimRotation = MazeHunterCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MazeHunterCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 15.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = MazeHunterCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);
}
