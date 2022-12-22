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
	//Actor�� beginPlay�� ����
	virtual void NativeConstruct() override;

public:
	//����� ��ư
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btnRestart;
	//���� ��ư
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btnExit;

	//��õ�
	UFUNCTION()
	void Retry();
	UFUNCTION()
	void Quit();
};
