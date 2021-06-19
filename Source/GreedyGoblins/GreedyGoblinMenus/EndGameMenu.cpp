// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameMenu.h"

#include "Components/Button.h"
#include "GreedyGoblins/MenuSystem/MenuInterface.h"

bool UEndGameMenu::Initialize(){
	bool Success = Super::Initialize();
	if (!Success) return false;

	if(!ensure(QuitButton)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UEndGameMenu::Quit);

	return true;
}

void UEndGameMenu::Quit(){
	if(!ensure(MenuInterface)) return;
	this->Teardown();
	MenuInterface->LoadMainMenu();
}
