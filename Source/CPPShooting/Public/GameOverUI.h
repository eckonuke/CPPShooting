// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUI.generated.h"

/**
 * 
 */
UCLASS()
class CPPSHOOTING_API UGameOverUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//Actor의 beginPlay와 같다
	virtual void NativeConstruct() override;

public:
	//재시작 버튼
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btnRestart;
	//종료 버튼
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btnExit;

	//재시도
	UFUNCTION()
	void Retry();
	UFUNCTION()
	void Quit();
};
