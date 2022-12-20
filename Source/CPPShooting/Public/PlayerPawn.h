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
	UPROPERTY(EditAnywhere)
	float speed = 500;
	//delay Time
	UPROPERTY(EditAnywhere)
	float delayTime = 0.3f;
	//Current Time
	UPROPERTY(EditAnywhere)
	float currTime = 0;
	//Box Component
	UPROPERTY(EditAnywhere)
	class UBoxComponent* compBox;
	//Mesh Component
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;
	//bulletFactory
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;
	//발사되지 않은 총알 탄창
	UPROPERTY(EditAnywhere)
	TArray<class ABullet*> mag;
	TArray<class ABullet*> arrayFired;
	//탄창 초기 갯수
	UPROPERTY(EditAnywhere)
	int32 bulletFirstCount = 10;
private:
	//사용자의 입력값을 받는 변수
	float h;
	float v;

	//사용자의 입력을 처리해주는 함수
	void MoveHorizontal(float value);
	void MoveVertical(float value);
	//총알 발사 입력 처리 함수
	void InputFire();
};
