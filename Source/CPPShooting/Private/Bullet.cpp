// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//box component 추가
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	//box component를 Root로 변경
	SetRootComponent(boxComp);
	//box의 초기 크기 설정
	boxComp->SetBoxExtent(FVector(25.0f, 25.0f, 50.0f));
	//Collision Preset을 Bullet Preset으로 한다
	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));


	//static mesh 추가
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	//static mesh를 box component 하위로 추가 
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	//Collision Preset을 NoCollision으로 한다
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	
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

	//만약에 mesh가 안보이는 상태라면 (비활성화 상태)
	if (meshComp->IsVisible() == false) {
		//함수를 나간다
		return;
	}

	//로직 자체는 플레이어가 이동하는 방식과 같다.
	//단지 플레이어는 입력 기반의 움직임이고 총알은 그냥 한 방향으로 움직인다
	FVector p0 = GetActorLocation();
	FVector p = p0 + GetActorUpVector() * DeltaTime * speed;
	SetActorLocation(p);
}

void ABullet::setActive(bool isActive) {
	if (isActive == true) {
		//보이게 한다
		meshComp->SetVisibility(true);
		//충돌 할 수 있게 하자
		boxComp->SetCollisionProfileName(TEXT("BulletPreset"));
	}
	else {
		//생성된 총알을 보이지 않게 한다
		meshComp->SetVisibility(false);
		//생선된 총알의 충돌 옵션을 NoCollision으로 한다
		boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}
}