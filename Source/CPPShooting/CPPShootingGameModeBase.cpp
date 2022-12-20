// Copyright Epic Games, Inc. All Rights Reserved.


#include "CPPShootingGameModeBase.h"
#include "MainUI.h"
#include <Blueprint/UserWidget.h>

ACPPShootingGameModeBase::ACPPShootingGameModeBase() {

}

void ACPPShootingGameModeBase::BeginPlay() {
	//mainUI�� �����
	mainUI = CreateWidget<UMainUI>(GetWorld(), mainWidget);
	//���� UI�� Viewport�� ������
	mainUI->AddToViewport();
}

void ACPPShootingGameModeBase::AddScore(int32 value) {
	score += value;
	UE_LOG(LogTemp, Warning, TEXT("Current Score: %d"), score);
}