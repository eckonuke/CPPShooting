// Copyright Epic Games, Inc. All Rights Reserved.


#include "CPPShootingGameModeBase.h"
#include "MainUI.h"
#include <Blueprint/UserWidget.h>

ACPPShootingGameModeBase::ACPPShootingGameModeBase() {

}

void ACPPShootingGameModeBase::BeginPlay() {
	//mainUI를 만든다
	mainUI = CreateWidget<UMainUI>(GetWorld(), mainWidget);
	//만든 UI를 Viewport에 붙힌다
	mainUI->AddToViewport();
}

void ACPPShootingGameModeBase::AddScore(int32 value) {
	score += value;
	UE_LOG(LogTemp, Warning, TEXT("Current Score: %d"), score);
}