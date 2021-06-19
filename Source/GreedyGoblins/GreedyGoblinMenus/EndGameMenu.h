// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GreedyGoblins/MenuSystem/MenuWidget.h"

#include "EndGameMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API UEndGameMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
private:

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UFUNCTION()
	void Quit();
};
