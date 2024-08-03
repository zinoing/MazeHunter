// Fill out your copyright notice in the Description page of Project Settings.


#include "OnHandComponent.h"
#include "Net/UnrealNetwork.h"
#include "MazeHunter/Item/Item.h"
#include "MazeHunter/Character/MazeHunterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"

UOnHandComponent::UOnHandComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOnHandComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UOnHandComponent, EquippedItem);
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
	EquippedItem->ShowPickupWidget(false);
	EquippedItem->GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// Called when the game starts
void UOnHandComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UOnHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

