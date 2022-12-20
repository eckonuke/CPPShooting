// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//������� include �� ���� �ϸ� �����Ͻÿ� �����ð��� �ɸ���
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
	//�߻���� ���� �Ѿ� źâ
	UPROPERTY(EditAnywhere)
	TArray<class ABullet*> mag;
	TArray<class ABullet*> arrayFired;
	//źâ �ʱ� ����
	UPROPERTY(EditAnywhere)
	int32 bulletFirstCount = 10;
private:
	//������� �Է°��� �޴� ����
	float h;
	float v;

	//������� �Է��� ó�����ִ� �Լ�
	void MoveHorizontal(float value);
	void MoveVertical(float value);
	//�Ѿ� �߻� �Է� ó�� �Լ�
	void InputFire();
};
