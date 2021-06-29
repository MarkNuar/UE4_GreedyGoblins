// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BoatHUD.generated.h"

class UImage;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API UBoatHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void ToggleMode(bool bIsInFastMode) const;
	
private:
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ImageSwitcher;
	
};
