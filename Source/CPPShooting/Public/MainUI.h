// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class CPPSHOOTING_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//Actor�� beginPlay�� ����
	virtual void NativeConstruct() override;

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* currScoreUI;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* bestScoreUI;

	void UpdateCurrScoreUI(int32 score);
	void UpdateBestScoreUI(int32 best);

};