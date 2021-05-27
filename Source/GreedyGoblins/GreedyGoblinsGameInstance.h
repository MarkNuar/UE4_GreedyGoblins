// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"

#include "GreedyGoblinsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API UGreedyGoblinsGameInstance : public UGameInstance, public IMenuInterface 
{
	GENERATED_BODY()


public:
	UGreedyGoblinsGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable) // called from blueprint
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable) // called from blueprint
	void InGameLoadMenu();

	UFUNCTION(Exec)
	void Host(FString ServerName) override;

	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	void StartSession();

	UFUNCTION(Exec)
	virtual void LoadMainMenu() override;

	virtual void ExitGame() override;

	virtual void RefreshServerList() override;


private:
	TSubclassOf<class UUserWidget> MenuClass;
	
	UPROPERTY()
	class UMainMenu* Menu;

	TSubclassOf<class UUserWidget> InGameMenuClass;
	
	UPROPERTY()
	class UInGameMenu* InGameMenu;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success); // No UFUNCTION() needed since it is not a single dynamic binding

	void OnDestroySessionComplete(FName SessionName, bool Success);

	void OnFindSessionComplete(bool Success);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& Error);

	FString DesiredServerName;
	
	void CreateSession();
};
