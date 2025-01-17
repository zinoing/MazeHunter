// Fill out your copyright notice in the Description page of Project Settings.


#include "OnHandComponent.h"
#include "Net/UnrealNetwork.h"
#include "MazeHunter/Item/Item.h"
#include "MazeHunter/Character/MazeHunterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UOnHandComponent::UOnHandComponent() : bAiming(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOnHandComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UOnHandComponent, EquippedItem);
	DOREPLIFETIME(UOnHandComponent, bAiming);
}

void UOnHandComponent::EquipItem(AItem* OverlappingItem)
{
	EquippedItem = OverlappingItem;
	EquippedItem->SetItemState(EItemState::EIS_Equipped);

	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket) {
		HandSocket->AttachActor(EquippedItem, Character->GetMesh());
	}

	EquippedItem->SetOwner(Character);
	// EquippedItem->ShowPickupWidget(false);
	// EquippedItem->GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}


void UOnHandComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UOnHandComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? Character->GetAimWalkSpeed() : Character->GetBaseWalkSpeed();
	}
}

void UOnHandComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character) {
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? Character->GetAimWalkSpeed() : Character->GetBaseWalkSpeed();
	}
}

void UOnHandComponent::OnRep_EuippedItem()
{
	if (EquippedItem && Character) {
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

void UOnHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

