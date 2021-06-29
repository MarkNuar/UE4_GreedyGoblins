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

void ABoatController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if(HUD)
	{
		HUD->RemoveFromViewport();
	}
}

void ABoatController::ToggleFastMode(bool bIsInFastMode) const
{
	if(HUD)
	{
		HUD->ToggleMode(bIsInFastMode);
	}
}

void ABoatController::RemoveHUD() const
{
	if(HUD)
	{
		HUD->RemoveFromViewport();
	}
}


