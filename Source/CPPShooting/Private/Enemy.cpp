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

	//������ ���� �̴´� (1, 100)
	//���࿡ ������ ���� 50���� ������(50%) �÷��̾ ã�´�
	int32 rand = FMath::RandRange(1, 100);

	if (rand < 50) {
		//1. �÷��̾ ã��
		AActor* player = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPawn::StaticClass());
		//2. �÷��̾ ���ϴ� ������ ������ (Ÿ�� - ��)
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

