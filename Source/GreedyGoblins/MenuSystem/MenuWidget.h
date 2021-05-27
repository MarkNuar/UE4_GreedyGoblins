// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class IMenuInterface;
/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup();
	void Teardown();

	void SetMenuInterface(IMenuInterface* MenuInterface);

protected:
	IMenuInterface* MenuInterface;
};
