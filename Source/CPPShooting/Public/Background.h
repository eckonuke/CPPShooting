// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Background.generated.h"

UCLASS()
class CPPSHOOTING_API ABackground : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABackground();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Static Mesh 안에 Plane이라는 모양을 설정해준다
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;
	UPROPERTY(EditAnywhere)
	float speed = 500;
};