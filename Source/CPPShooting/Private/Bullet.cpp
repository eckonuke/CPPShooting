// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//box component �߰�
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	//box component�� Root�� ����
	SetRootComponent(boxComp);
	//box�� �ʱ� ũ�� ����
	boxComp->SetBoxExtent(FVector(25.0f, 25.0f, 50.0f));

	//static mesh �߰�
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	//static mesh�� box component ������ �߰� 
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���� ��ü�� �÷��̾ �̵��ϴ� ��İ� ����.
	//���� �÷��̾�� �Է� ����� �������̰� �Ѿ��� �׳� �� �������� �����δ�
	FVector p0 = GetActorLocation();
	FVector p = p0 + GetActorUpVector() * DeltaTime * speed;

	SetActorLocation(p);
}
