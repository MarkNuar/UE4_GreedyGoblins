// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatController.h"

#include "Blueprint/UserWidget.h"
#include "GreedyGoblins/GreedyGoblinMenus/BoatHUD.h"


void ABoatController::BeginPlay()
{
	Super::BeginPlay();

    if(IsLocalPlayerController())
    {
    	HUD = CreateWidget<UBoatHUD>(this, HUDClass);
    	if(HUD)
    	{
    		HUD->AddToViewport();
    	}
    }
}

void ABoatController::ToggleFastMode() const
{
	if(HUD)
	{
		HUD->ToggleMode();
	}
}

void ABoatController::RemoveHUD() const
{
	if(HUD)
	{
		HUD->RemoveFromViewport();
	}
}


