// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MenuWidget.h"

#include "MainMenu.generated.h"

USTRUCT()
struct FServerData{
	
	GENERATED_BODY()
	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayer;
	FString HostUsername;
	
};



class UEditableTextBox;
class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:

	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerData);

	void SelectIndex(uint32 Index);
	
protected:
	
	virtual bool Initialize() override;
	
private:
	
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitGameButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmHostMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelHostMenuButton;
	
	TSubclassOf<UUserWidget> ServerRowClass;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void Join();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void ExitGame();

	void UpdateChildren();

	TOptional<uint32> SelectedIndex;
};
