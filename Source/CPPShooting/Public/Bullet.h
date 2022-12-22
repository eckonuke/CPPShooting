// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroyBullet, ABullet*, bullet);

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

	void setActive(bool isActive);

	//ÃÑ¾Ë box component
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	//ÃÑ¾Ë static mesh component
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	//ÃÑ¾Ë ¼Óµµ ÁöÁ¤
	UPROPERTY(EditAnywhere)
	float speed = 1000;
	
	FDestroyBullet onDestroyBullet;
};
