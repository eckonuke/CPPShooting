// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include <Components/TextBlock.h>
#include <Components/CanvasPanelSlot.h>

void UMainUI::UpdateCurrScoreUI(int32 score) {
	//currScoreUI의 text값을 score 값으로 한다.
	currScoreUI->SetText(FText::AsNumber(score));

	//델리게잍트에 등록된 함수 실행
	onFirstDel.ExecuteIfBound();
	onSecondDel.ExecuteIfBound(score);
	onThirdDel.Broadcast(score);
}

void UMainUI::UpdateBestScoreUI(int32 best) {
	//bestScoreUI의 text값을 best 값으로 한다
	bestScoreUI->SetText(FText::AsNumber(best));
}

void UMainUI::NativeConstruct() {
	Super::NativeConstruct();
	//bestScoreUI의 색을 바꿔보자
	//R G B (0 ~ 255) or (0 ~ 1)
	currScoreUI->SetColorAndOpacity(FLinearColor(FVector3f(0, 1, 1)));
	bestScoreUI->SetColorAndOpacity(FLinearColor(FVector3f(0, 1, 1)));

	//위치 X값만 500으로 바꾸자
	//slot을 가져오자
	UPanelSlot* slot = bestScoreUI->Slot;
	//Canvas Panel Slot으로 Casting 해주자
	UCanvasPanelSlot * canvas = Cast<UCanvasPanelSlot>(slot);
	//위치를 바꾸자
	//canvas->SetPosition(FVector2D(500, 100));

	//델리게이트에 함수등록
	onFirstDel.BindUObject(this, &UMainUI::FuncFirstDel);
	onSecondDel.BindUFunction(this, TEXT("FuncSecondDel"));
	onThirdDel.AddUObject(this, &UMainUI::FuncThirdDel);
	onThirdDel.AddUObject(this, &UMainUI::FuncThirdDel2);
	onThirdDel.AddUObject(this, &UMainUI::FuncSecondDel);
}

void UMainUI::FuncFirstDel() {
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
}
void UMainUI::FuncSecondDel(int32 number) {
	UE_LOG(LogTemp, Warning, TEXT("%s %d"), *FString(__FUNCTION__), number);
}

void UMainUI::FuncThirdDel(int32 number) {
	UE_LOG(LogTemp, Warning, TEXT("%s %d"), *FString(__FUNCTION__), number);
}
void UMainUI::FuncThirdDel2(int32 number) {
	UE_LOG(LogTemp, Warning, TEXT("%s %d"), *FString(__FUNCTION__), number);
}