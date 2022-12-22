// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

DECLARE_DELEGATE(FFirstDel);
DECLARE_DELEGATE_OneParam(FSecondDel, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FThirdDel, int32);
/**
 * 
 */
UCLASS()
class CPPSHOOTING_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//Actor의 beginPlay와 같다
	virtual void NativeConstruct() override;

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* currScoreUI;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* bestScoreUI;

	void UpdateCurrScoreUI(int32 score);
	void UpdateBestScoreUI(int32 best);

public:
	//Delegate 연습
	FFirstDel onFirstDel;
	UFUNCTION()
	void FuncFirstDel();

	FSecondDel onSecondDel;
	UFUNCTION()
	void FuncSecondDel(int32 number);

	FThirdDel onThirdDel;
	UFUNCTION()
	void FuncThirdDel(int32 number);
	UFUNCTION()
	void FuncThirdDel2(int32 number);
};