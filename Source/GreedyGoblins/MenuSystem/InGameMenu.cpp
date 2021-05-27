// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

#include "MenuInterface.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize(){
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if(!ensure(CancelButton)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::ReturnToGame);

	if(!ensure(QuitButton)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::Quit);

	return true;
}

void UInGameMenu::ReturnToGame(){
	this->Teardown();
}

void UInGameMenu::Quit(){
	if(!ensure(MenuInterface)) return;
	this->Teardown();
	MenuInterface->LoadMainMenu();
}
