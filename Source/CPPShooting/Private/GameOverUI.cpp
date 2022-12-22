// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"
#include <Kismet/GameplayStatics.h>
#include <Components/Button.h>


void UGameOverUI::NativeConstruct() {
	Super::NativeConstruct();

	//btnRetry Ŭ���� Retry �Լ� ����
	btnRestart->OnClicked.AddDynamic(this, &UGameOverUI::Retry);
	//btnQuit Ŭ���� Quit �Լ� ����
	btnExit->OnClicked.AddDynamic(this, &UGameOverUI::Quit);
}

void UGameOverUI::Retry() {
	//������ �ٽ� �ε��Ѵ�
	UGameplayStatics::OpenLevel(GetWorld(), "ShootingMap");
}

void UGameOverUI::Quit() {
	//���� �����Ų��
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}