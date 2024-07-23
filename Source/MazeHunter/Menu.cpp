	// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"

void UMenu::MenuSetup(FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);

	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World) {
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			FInputModeUIOnly UI_InputModeData;
			UI_InputModeData.SetWidgetToFocus(TakeWidget());
			UI_InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			PlayerController->SetInputMode(UI_InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize()) {
		return false;
	}

	if (PlayButton) {
		PlayButton->OnClicked.AddDynamic(this, &ThisClass::PlayButtonClicked);
	}
	return true;
}

void UMenu::PlayButtonClicked()
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Blue,
			FString(TEXT("Play Button Clicked!"))
		);
	}

	if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel(PathToLobby, TRAVEL_Absolute);
	}
}
