// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LighthouseAIController.generated.h"

/**
* 
*/
UCLASS()
class GREEDYGOBLINS_API ALighthouseAIController : public AAIController
{
	GENERATED_BODY()

	public:
	virtual void Tick(float DeltaSeconds) override;
  
	protected:
	void BeginPlay() override;

	private:
	
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
};