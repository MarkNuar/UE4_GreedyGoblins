// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "MenuInterface.h"
#include "ServerRow.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer){
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class)) return;
	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::SetServerList(TArray<FServerData> ServerData){
	
	UWorld* World = GetWorld();
	if (!ensure(World)) return;
	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerName : ServerData){
		if(!ensure(ServerRowClass)) return; 
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if(!ensure(Row)) return;
		Row->Setup(this, i);
		i++;
		Row->ServerName->SetText(FText::FromString(ServerName.Name));
		Row->HostUser->SetText(FText::FromString(ServerName.HostUsername));
		Row->ConnectionFraction->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"),ServerName.CurrentPlayers ,ServerName.MaxPlayer )));
		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index){
	SelectedIndex = Index;
	UpdateChildren();
}

bool UMainMenu::Initialize(){
	bool Success = Super::Initialize();
	if (!Success) return false;

	if(!ensure(HostButton)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if(!ensure(JoinButton)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if(!ensure(ConfirmJoinMenuButton)) return false;
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::Join);

	if(!ensure(CancelJoinMenuButton)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if(!ensure(ConfirmHostMenuButton)) return false;
	ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	
	if(!ensure(CancelHostMenuButton)) return false;
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if(!ensure(ExitGameButton)) return false;
	ExitGameButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);

	return true;
}

void UMainMenu::HostServer(){
	if (!ensure(MenuInterface)) return;
	FString ServerName = ServerHostName->Text.ToString();
	MenuInterface->Host(ServerName);
}

void UMainMenu::Join(){
	if(MenuInterface && SelectedIndex.IsSet()){
		UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}else{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index no"));
	}
	
	
}

void UMainMenu::OpenJoinMenu(){
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(JoinMenu)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface){
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenHostMenu(){
	if (!ensure(MenuSwitcher)) return;
    if (!ensure(HostMenu)) return;
    MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::OpenMainMenu(){
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(MainMenu)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::ExitGame(){
	if (!ensure(MenuInterface)) return;
	MenuInterface->ExitGame();
}

void UMainMenu::UpdateChildren(){
	for(int32 i = 0; i<ServerList->GetChildrenCount(); i++){
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if(Row && SelectedIndex.IsSet()){
			Row->Selected = i == SelectedIndex.GetValue();
		}
	}
}
