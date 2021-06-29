// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BoatController.generated.h"

class UBoatHUD;
/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API ABoatController : public APlayerController
{
	GENERATED_BODY()

public:
	void ToggleFastMode(bool bIsInFastMode) const;

	void RemoveHUD() const;

protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY()
	UBoatHUD* HUD = nullptr;

};
