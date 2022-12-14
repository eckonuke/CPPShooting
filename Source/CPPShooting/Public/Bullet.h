// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class CPPSHOOTING_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�Ѿ� �ӵ� ����
	UPROPERTY(EditAnywhere)
	float speed = 1000;
	//�Ѿ� box component
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	//�Ѿ� static mesh component
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
};
