// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class CPPSHOOTING_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Event when overlap
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	FVector dir;

	//Mesh Component
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	//Box Component
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	//ÀÌµ¿¼Óµµ
	UPROPERTY(EditAnywhere)
	float speed = 400;
	//Æø¹ßÈ¿°ú
	UPROPERTY(EditAnywhere)
	class UParticleSystem* explodeFactory;
	//ÆøÆÈÀ½
	UPROPERTY(EditAnywhere)
	class USoundBase* explodeSound;
	UPROPERTY(EditAnywhere)
	class APlayerPawn* playerPawn;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);
};
