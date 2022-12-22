// Fill out your copyright notice in the Description page of Project Settings.


#include "KillBox.h"
#include <Components/BoxComponent.h>
#include "Bullet.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerPawn.h"

// Sets default values
AKillBox::AKillBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50, 2000, 50));
	boxComp->SetCollisionProfileName(TEXT("KillBoxPreset"));
}

// Called when the game starts or when spawned
void AKillBox::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AKillBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillBox::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->GetName().Contains(TEXT("Bullet"))) {
		//부딪힌 놈이 Bullet 이면 
		//OtherActor를 Bullet으로 형변환
		ABullet* bullet = Cast<ABullet>(OtherActor);
		// 비활성화 하자
		bullet->setActive(false);
		// 탄창에 다시 넣자
		bullet->onDestroyBullet.Broadcast(bullet);
		// APlyerPawn을 찾자
	//	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPawn::StaticClass());
	//	APlayerPawn* player = Cast<APlayerPawn>(actor);
	//	// 탄창에 다시 넣자
	//	player->mag.Add(bullet);
	}
	else {// 그렇지 않으면 파괴하자

		//부딪힌 놈을 파괴하자
		OtherActor->Destroy();

	}

}