// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeHunterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "MazeHunter/Item/Item.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"

AMazeHunterCharacter::AMazeHunterCharacter()
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
}


void AMazeHunterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMazeHunterCharacter, OverlappingItem, COND_OwnerOnly);
}

void AMazeHunterCharacter::BeginPlay()
{
	Super::BeginPlay();
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
	if (EquippedItem)
		return true;
	return false;
}

void AMazeHunterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ThisClass::EquipItem);

	PlayerInputComponent->BindAxis("MoveForward", this,  &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
}

void AMazeHunterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f) {
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void AMazeHunterCharacter::EquipItem_Implementation()
{
	if (OverlappingItem == nullptr) return;

	/*if (HasAuthority()) {
		EquippedItem = OverlappingItem;
		EquippedItem->SetItemState(EItemState::EIS_Equipped);

		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if (HandSocket) {
			HandSocket->AttachActor(EquippedItem, GetMesh());
		}

		EquippedItem->ShowPickupWidget(false);
		EquippedItem->GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else {
		ServerEquipItem();
	}*/
	EquippedItem = OverlappingItem;
	EquippedItem->SetItemState(EItemState::EIS_Equipped);

	const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket) {
		HandSocket->AttachActor(EquippedItem, GetMesh());
	}

	EquippedItem->ShowPickupWidget(false);
	EquippedItem->GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
}
