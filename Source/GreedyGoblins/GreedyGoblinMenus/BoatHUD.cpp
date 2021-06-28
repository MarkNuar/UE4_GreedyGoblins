// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatHUD.h"

#include "Components/WidgetSwitcher.h"


void UBoatHUD::ToggleMode() const
{
	if(!ensure(ImageSwitcher)) return;
	if(ImageSwitcher->GetActiveWidgetIndex() == 0)
	{
		ImageSwitcher->SetActiveWidgetIndex(1);
	}
	else
	{
		ImageSwitcher->SetActiveWidgetIndex(0);
	}

}
