// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerPawn.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	boxComp->SetBoxExtent(FVector(50));
	SetRootComponent(boxComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	meshComp->SetupAttachment(boxComp);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//랜덤한 값을 뽑는다 (1, 100)
	//만약에 랜덤한 값이 50보다 작으면(50%) 플레이어를 찾는다
	int32 rand = FMath::RandRange(1, 100);

	if (rand < 50) {
		//1. 플레이어를 찾자
		AActor* player = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPawn::StaticClass());
		//2. 플레이어를 향하는 방향을 구하자 (타겟 - 나)
		dir = player->GetActorLocation() - GetActorLocation();
		//3. Normalize
		dir.Normalize();
	}
	else {
		dir = -GetActorUpVector();
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//P = p0 + vt
	FVector p0 = GetActorLocation();
	FVector vt = dir * DeltaTime * speed;
	SetActorLocation(p0 + vt);
}

