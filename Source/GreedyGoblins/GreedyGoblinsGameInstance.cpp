// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyGoblinsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

const static FName SESSION_NAME = TEXT("Game");
static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UGreedyGoblinsGameInstance::UGreedyGoblinsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu")); // gets hold of the blueprint widget
	if(!ensure(MenuBPClass.Class!=nullptr)) return;
	MenuClass = MenuBPClass.Class; // store it in a local variable, private

	const ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu")); // gets hold of the blueprint widget
	if(!ensure(InGameMenuBPClass.Class!=nullptr)) return;
	InGameMenuClass = InGameMenuBPClass.Class; // store it in a local variable, private
}

void UGreedyGoblinsGameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface(); // Interface of the online subsystem
		if (SessionInterface.IsValid())
		{
			// callback called when the SessionInterface->CreateSession() completes
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGreedyGoblinsGameInstance::OnCreateSessionComplete); // When CreateSession completes, use this object. 
			// callback called when the SessionInterface->DestroySession() completes
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UGreedyGoblinsGameInstance::OnDestroySessionComplete);
			// callback called when the SessionInterface->FindSessions() completes
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGreedyGoblinsGameInstance::OnFindSessionComplete);
			// callback called when the SessionInterface->JoinSession() completes
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGreedyGoblinsGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}

	if(GEngine!=nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UGreedyGoblinsGameInstance::OnNetworkFailure);
	}
}

void UGreedyGoblinsGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver,
	ENetworkFailure::Type FailureType, const FString& Error)
{
	this->LoadMainMenu();
}

void UGreedyGoblinsGameInstance::LoadMenuWidget() // called from the blueprint of the main menu level, after begin play 
{
	if(!ensure(MenuClass!=nullptr)) return;
	
	Menu = CreateWidget<UMainMenu>(this,MenuClass); // instantiate the menu widget
	if(!ensure(Menu!=nullptr)) return;
	// SETUP THE MENU (NO CURSOR, SHOW, ETC..)
	Menu->Setup();
	// SET THE MENU INTERFACE IN THE MAIN MENU
	Menu->SetMenuInterface(this);
}

void UGreedyGoblinsGameInstance::InGameLoadMenu() // called from the blueprint of the level, on esc press
{
	if(!ensure(InGameMenuClass!=nullptr)) return;
	
	InGameMenu = CreateWidget<UInGameMenu>(this,InGameMenuClass); // instantiate the menu widget
	if(!ensure(InGameMenu!=nullptr)) return;
	UE_LOG(LogTemp, Warning, TEXT("In game load menu"));
	// SETUP THE MENU (NO CURSOR, SHOW, ETC..)
	InGameMenu->Setup();
	// SET THE MENU INTERFACE IN THE MAIN MENU
	InGameMenu->SetMenuInterface(this);
}

void UGreedyGoblinsGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController!=nullptr)) return;
	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu",ETravelType::TRAVEL_Absolute);
}

void UGreedyGoblinsGameInstance::ExitGame()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController!=nullptr)) return;
	PlayerController->ConsoleCommand("Quit");
}

void UGreedyGoblinsGameInstance::Host(FString ServerName) // called from the client trying to host. clicking host button
{
	DesiredServerName = ServerName;
	if(SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME); // return the session with the specified name
		if(ExistingSession != nullptr) // if the session already exists, destroy it and create a new one where I'm the host
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroying previous session"));
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else // if the session doesn't exists
		{
			CreateSession();
		}
	}		
}

void UGreedyGoblinsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if(Success)
	{
		CreateSession();
	}
}

void UGreedyGoblinsGameInstance::CreateSession()
{
	if(SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		if(IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set<FString>(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings); // this call is async, since it traverse the network
	}
}

void UGreedyGoblinsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if(!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session with the given name already exists")); // this may happens if a session with the given name already exists 
		return;	
	}
	if(Menu!=nullptr)
	{
		Menu->Teardown();
	}
	UEngine* Engine = GetEngine();
	if(!ensure(Engine!=nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if(!ensure(World!=nullptr)) return;
	World->ServerTravel("/Game/Levels/Lobby?listen"); // listen -> we are waiting for clients 
}

void UGreedyGoblinsGameInstance::Join(uint32 Index)
{
	if(!SessionInterface.IsValid()) return;
	if(!SessionSearch.IsValid()) return;
	
	if(Menu!=nullptr)
	{
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]); // join the session at the given index in the session search search results
}

void UGreedyGoblinsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(!SessionInterface.IsValid()) return;

	FString Address;
	if(!SessionInterface->GetResolvedConnectString(SessionName, Address)) // Address is an out parameter
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}
	
	UEngine* Engine = GetEngine();
	if(!ensure(Engine!=nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController!=nullptr)) return;
	PlayerController->ClientTravel(Address,ETravelType::TRAVEL_Absolute);
}

void UGreedyGoblinsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if(SessionSearch.IsValid())
	{
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); // settings for search presence
		SessionSearch->MaxSearchResults = 100; // get enough results back
		UE_LOG(LogTemp, Warning, TEXT("Started finding session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UGreedyGoblinsGameInstance::OnFindSessionComplete(bool Success)
{
	if(Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find session completed"));

		TArray<FServerData> ServerDatas;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session: %s"), *SearchResult.GetSessionIdStr())
			FServerData Data;
			
			Data.MaxPlayer = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayer - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;
			FString ServerName;
			if(SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Could not find name.";
			}
			ServerDatas.Add(Data);
		}
		Menu->SetServerList(ServerDatas); // add the found session strings to the Menu
	}
}

void UGreedyGoblinsGameInstance::StartSession()
{
	if(SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME); // Marks an online session as in progress (as opposed to being in lobby or pending)
	}
}