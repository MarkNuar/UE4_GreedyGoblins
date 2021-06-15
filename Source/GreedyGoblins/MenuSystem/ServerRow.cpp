// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "MainMenu.h"
#include "Components/Button.h"

void UServerRow::Setup(UMainMenu* ParentPar, uint32 IndexPar){
	if(!ensure(RowButton)) return;
	Parent = ParentPar;
	Index = IndexPar;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClick);
}

void UServerRow::OnClick(){
	Parent->SelectIndex(Index);
}
