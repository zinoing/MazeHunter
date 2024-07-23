// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MAZEHUNTER_API UMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(FString LobbyPath);

protected:
	virtual bool Initialize() override;

private:

	UPROPERTY( meta = (BindWidget))
	class UButton* PlayButton;
	
	UFUNCTION()
	void PlayButtonClicked();

	FString PathToLobby;
};
