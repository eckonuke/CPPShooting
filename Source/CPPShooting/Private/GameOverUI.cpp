// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"
#include <Kismet/GameplayStatics.h>
#include <Components/Button.h>


void UGameOverUI::NativeConstruct() {
	Super::NativeConstruct();

	//btnRetry 클릭시 Retry 함수 연결
	btnRestart->OnClicked.AddDynamic(this, &UGameOverUI::Retry);
	//btnQuit 클릭시 Quit 함수 연결
	btnExit->OnClicked.AddDynamic(this, &UGameOverUI::Quit);
}

void UGameOverUI::Retry() {
	//레벨을 다시 로드한다
	UGameplayStatics::OpenLevel(GetWorld(), "ShootingMap");
}

void UGameOverUI::Quit() {
	//앱을 종료시킨다
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}