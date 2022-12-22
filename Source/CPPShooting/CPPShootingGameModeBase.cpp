// Copyright Epic Games, Inc. All Rights Reserved.


#include "CPPShootingGameModeBase.h"
#include "MainUI.h"
#include <Blueprint/UserWidget.h>
#include "BestScoreData.h"
#include <Kismet/GameplayStatics.h>
#include "GameOverUI.h"

ACPPShootingGameModeBase::ACPPShootingGameModeBase() {
	//BlueprintŬ������ ��� ������ �κ� ���� ����ǥ �ڿ� _C�� �߰��ؾ� �����ü� �ִ�
	ConstructorHelpers::FClassFinder<UMainUI> tempMainUI(TEXT("WidgetBlueprint'/Game/Blueprints/BP_MainUI.BP_MainUI_C'"));
	if (tempMainUI.Succeeded()) {
		mainWidget = tempMainUI.Class;
	}
	ConstructorHelpers::FClassFinder<UGameOverUI> tempGameOverUI(TEXT("WidgetBlueprint'/Game/Blueprints/BP_GameOver.BP_GameOver_C'"));
	if (tempGameOverUI.Succeeded()) {
		gameOverWidget = tempGameOverUI.Class;
	}
}

void ACPPShootingGameModeBase::BeginPlay() {
	Super::BeginPlay();
	//mainUI�� �����
	mainUI = CreateWidget<UMainUI>(GetWorld(), mainWidget);
	//���� UI�� Viewport�� ������
	mainUI->AddToViewport();
	//����� ������ �ε� �Ѵ�
	LoadBestScore();
}

void ACPPShootingGameModeBase::ShowGameOverUI() {
	//GameOverui�� �ϳ� �����
	gameOverUI = CreateWidget<UGameOverUI>(GetWorld(), gameOverWidget);
	//���� UI�� ȭ�鿡 ����
	gameOverUI->AddToViewport();
}

void ACPPShootingGameModeBase::AddScore(int32 value) {
	currScore += value;
	mainUI->UpdateCurrScoreUI(currScore);
	//�ְ����� ���� ����(�������� > �ְ�����)
	//�����ϸ� �ְ� ���� ����
	//�ְ����� ���
	if (currScore > bestScore) {
		bestScore = currScore;
		mainUI->UpdateBestScoreUI(bestScore);
	}
	//bestScore ����
	SaveBestScore();
}

void ACPPShootingGameModeBase::SaveBestScore() {
	//bestScore -> bestScoreData�� �ִ� saveBestScore ������ �ִ´�.

	//1. UBestScoreData�� �ϳ� �����Ѵ�. (USaveGame*)
	USaveGame* saveGame = UGameplayStatics::CreateSaveGameObject(UBestScoreData::StaticClass());
	//2. ������ ���� UBestSoreData�� Cast����
	UBestScoreData* bestScoreData = Cast<UBestScoreData>(saveGame);
	//3. saveBestScore = bestScore
	bestScoreData->saveBestScore = bestScore;
	//4. �����Ѵ�
	UGameplayStatics::SaveGameToSlot(bestScoreData, TEXT("BestScore"), 0);
}
void ACPPShootingGameModeBase::LoadBestScore() {
	//����Ǿ��ִ� ������ �ҷ��´�

	//1. "BestScore"�ҷ��´� (USavmeGame*)
	USaveGame* loadGame = UGameplayStatics::LoadGameFromSlot(TEXT("BestScore"), 0);
	//2. �ҷ��� ���� UBestScoreData�� Cast����
	UBestScoreData* bestScoreData = Cast<UBestScoreData>(loadGame);

	//�ҷ��� �����Ͱ� �����Ҷ��� bestScore�� �����Ѵ�
	if (bestScoreData != nullptr) {
		//3. bestScore = saveBestScore
		bestScore = bestScoreData->saveBestScore;
		//4. BestScoreUI�� �����Ѵ�
		mainUI->UpdateBestScoreUI(bestScore);
	}
}

void ACPPShootingGameModeBase::ShuffleStudy() {
	//int�� 100�� ���� �� �ִ� �迭�� �����.
	//�ʱⰪ�� 1 ~ 100 ���� ���� �Ѵ�.
	//�ȿ� ������ ����!!
	//�迭 ���� ���� ����غ���.
	int array[100];
	int size = sizeof(array) / sizeof(array[0]);
	for (int i = 0; i < size; i++)
	{
		array[i] = i + 1;
	}

	for (int i = 0; i < 100; i++) {
		//������ ���� �̴´� (0~4)
		int32 randA = FMath::RandRange(0, 99);
		int32 randB = FMath::RandRange(0, 99);
		int num = array[randA];
		array[randA] = array[randB];
		array[randB] = num;
	}

	for (int i = 0; i < size; i++) {
		UE_LOG(LogTemp, Warning, TEXT("%d"), array[i]);
	}
}