// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//헤더에는 include 를 많이 하면 컴파일시에 오랜시간이 걸린다
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class CPPSHOOTING_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//player speed
	float speed = 500;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;

private:
	//사용자의 입력값을 받는 변수
	float h;
	float v;

	//사용자의 입력을 처리해주는 함수
	void MoveHorizontal(float value);
	void MoveVertical(float value);
};
