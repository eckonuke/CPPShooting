// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPPShootingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CPPSHOOTING_API ACPPShootingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	ACPPShootingGameModeBase();

	void AddScore(int32 value);
	void SaveBestScore();
	void LoadBestScore();
	void ShowGameOverUI();

	void ShuffleStudy();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainUI> mainWidget;
	UPROPERTY()
	class UMainUI* mainUI;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameOverUI> gameOverWidget;
	UPROPERTY()
	class UGameOverUI* gameOverUI;

	//현재 점수
	int32 currScore = 0;
	//최고 점수
	int32 bestScore = 0;
private:
};
