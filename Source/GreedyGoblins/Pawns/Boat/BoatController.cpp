// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatController.h"

void ABoatController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(!ensure(InPawn)) return;
	if(HasAuthority())
		InPawn->SetAutonomousProxy(false);
}
