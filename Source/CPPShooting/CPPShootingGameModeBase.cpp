// Copyright Epic Games, Inc. All Rights Reserved.


#include "CPPShootingGameModeBase.h"
#include "MainUI.h"
#include <Blueprint/UserWidget.h>
#include "BestScoreData.h"
#include <Kismet/GameplayStatics.h>
#include "GameOverUI.h"

ACPPShootingGameModeBase::ACPPShootingGameModeBase() {
	//Blueprint클래스는 경로 마지막 부분 작은 따옴표 뒤에 _C를 추가해야 가져올수 있다
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
	//mainUI를 만든다
	mainUI = CreateWidget<UMainUI>(GetWorld(), mainWidget);
	//만든 UI를 Viewport에 붙힌다
	mainUI->AddToViewport();
	//저장된 게임을 로드 한다
	LoadBestScore();
}

void ACPPShootingGameModeBase::ShowGameOverUI() {
	//GameOverui를 하나 만든다
	gameOverUI = CreateWidget<UGameOverUI>(GetWorld(), gameOverWidget);
	//만든 UI를 화면에 띄운다
	gameOverUI->AddToViewport();
}

void ACPPShootingGameModeBase::AddScore(int32 value) {
	currScore += value;
	mainUI->UpdateCurrScoreUI(currScore);
	//최고점수 갱신 조건(현재점수 > 최고점수)
	//만족하면 최고 점수 갱신
	//최고점수 출력
	if (currScore > bestScore) {
		bestScore = currScore;
		mainUI->UpdateBestScoreUI(bestScore);
	}
	//bestScore 저장
	SaveBestScore();
}

void ACPPShootingGameModeBase::SaveBestScore() {
	//bestScore -> bestScoreData에 있는 saveBestScore 변수에 넣는다.

	//1. UBestScoreData를 하나 생성한다. (USaveGame*)
	USaveGame* saveGame = UGameplayStatics::CreateSaveGameObject(UBestScoreData::StaticClass());
	//2. 생성한 놈을 UBestSoreData로 Cast하자
	UBestScoreData* bestScoreData = Cast<UBestScoreData>(saveGame);
	//3. saveBestScore = bestScore
	bestScoreData->saveBestScore = bestScore;
	//4. 저장한다
	UGameplayStatics::SaveGameToSlot(bestScoreData, TEXT("BestScore"), 0);
}
void ACPPShootingGameModeBase::LoadBestScore() {
	//저장되어있는 게임을 불러온다

	//1. "BestScore"불러온다 (USavmeGame*)
	USaveGame* loadGame = UGameplayStatics::LoadGameFromSlot(TEXT("BestScore"), 0);
	//2. 불러온 놈을 UBestScoreData로 Cast하자
	UBestScoreData* bestScoreData = Cast<UBestScoreData>(loadGame);

	//불러온 데이터가 존재할때만 bestScore를 갱신한다
	if (bestScoreData != nullptr) {
		//3. bestScore = saveBestScore
		bestScore = bestScoreData->saveBestScore;
		//4. BestScoreUI를 갱신한다
		mainUI->UpdateBestScoreUI(bestScore);
	}
}

void ACPPShootingGameModeBase::ShuffleStudy() {
	//int를 100개 담을 수 있는 배열을 만든다.
	//초기값을 1 ~ 100 까지 들어가게 한다.
	//안에 내용을 섞자!!
	//배열 안의 값을 출력해보자.
	int array[100];
	int size = sizeof(array) / sizeof(array[0]);
	for (int i = 0; i < size; i++)
	{
		array[i] = i + 1;
	}

	for (int i = 0; i < 100; i++) {
		//랜덤한 값을 뽑는다 (0~4)
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