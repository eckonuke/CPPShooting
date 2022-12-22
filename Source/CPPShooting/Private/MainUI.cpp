// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include <Components/TextBlock.h>
#include <Components/CanvasPanelSlot.h>

void UMainUI::UpdateCurrScoreUI(int32 score) {
	//currScoreUI�� text���� score ������ �Ѵ�.
	currScoreUI->SetText(FText::AsNumber(score));

	//�����ԟ�Ʈ�� ��ϵ� �Լ� ����
	onFirstDel.ExecuteIfBound();
	onSecondDel.ExecuteIfBound(score);
	onThirdDel.Broadcast(score);
}

void UMainUI::UpdateBestScoreUI(int32 best) {
	//bestScoreUI�� text���� best ������ �Ѵ�
	bestScoreUI->SetText(FText::AsNumber(best));
}

void UMainUI::NativeConstruct() {
	Super::NativeConstruct();
	//bestScoreUI�� ���� �ٲ㺸��
	//R G B (0 ~ 255) or (0 ~ 1)
	currScoreUI->SetColorAndOpacity(FLinearColor(FVector3f(0, 1, 1)));
	bestScoreUI->SetColorAndOpacity(FLinearColor(FVector3f(0, 1, 1)));

	//��ġ X���� 500���� �ٲ���
	//slot�� ��������
	UPanelSlot* slot = bestScoreUI->Slot;
	//Canvas Panel Slot���� Casting ������
	UCanvasPanelSlot * canvas = Cast<UCanvasPanelSlot>(slot);
	//��ġ�� �ٲ���
	//canvas->SetPosition(FVector2D(500, 100));

	//��������Ʈ�� �Լ����
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