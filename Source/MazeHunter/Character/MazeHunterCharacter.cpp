// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeHunterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "MazeHunter/Item/Item.h"
#include "Engine/SkeletalMeshSocket.h"
#include "MazeHunter/MazeHunterComponents/OnHandComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

AMazeHunterCharacter::AMazeHunterCharacter() : BaseWalkSpeed(600.0f), AimWalkSpeed(450.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OnHand = CreateDefaultSubobject<UOnHandComponent>("OnHand");
	OnHand->SetIsReplicated(true);
	OnHand->Character = this;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}


void AMazeHunterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMazeHunterCharacter, OverlappingItem, COND_OwnerOnly);
}

void AMazeHunterCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AMazeHunterCharacter::OnRep_OverlappingItem(AItem* LastItem)
{
	if (OverlappingItem) {
		OverlappingItem->ShowPickupWidget(true);
	}

	if(LastItem) {
		LastItem->ShowPickupWidget(false);
	}
}

void AMazeHunterCharacter::SetOverlappingItem(AItem* Item)
{
	if (OverlappingItem) {
		OverlappingItem->ShowPickupWidget(false);
	}

	OverlappingItem = Item;

	if (IsLocallyControlled()) {
		if (OverlappingItem) {
			OverlappingItem->ShowPickupWidget(true);
		}
	}
}

bool AMazeHunterCharacter::IsItemEquipped()
{
	return (OnHand && OnHand->EquippedItem);
}

bool AMazeHunterCharacter::IsAiming()
{
	return (OnHand && OnHand->bAiming);
}

void AMazeHunterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ThisClass::EquipButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ThisClass::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ThisClass::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ThisClass::AimButtonReleased);

	PlayerInputComponent->BindAxis("MoveForward", this,  &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
}

void AMazeHunterCharacter::EquipButtonPressed()
{
	if (OverlappingItem == nullptr) return;

	if (OnHand) {
		if (HasAuthority()) {
			OnHand->EquipItem(OverlappingItem);
		}
		else {
			ServerEquipItem();
		}
	}
}

void AMazeHunterCharacter::CrouchButtonPressed()
{
	if (bIsCrouched) {
		Super::UnCrouch();
	}
	else {
		Super::Crouch();
	}
}

void AMazeHunterCharacter::AimButtonPressed()
{
	if (OnHand) {
		OnHand->SetAiming(true);
	}
}

void AMazeHunterCharacter::AimButtonReleased()
{
	if (OnHand) {
		OnHand->SetAiming(false);
	}
}

void AMazeHunterCharacter::AimOffset(float DeltaTime)
{
	if (OnHand && OnHand->EquippedItem == nullptr) return;

	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) { // standing still, not jumping
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		bUseControllerRotationYaw = false;
	}
	else {
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.0f && !IsLocallyControlled()) {
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void AMazeHunterCharacter::ServerEquipItem_Implementation()
{
	if (OnHand) {
		OnHand->EquipItem(OverlappingItem);
	}
}

void AMazeHunterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f) {
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void AMazeHunterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f) {
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void AMazeHunterCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AMazeHunterCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMazeHunterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);
}