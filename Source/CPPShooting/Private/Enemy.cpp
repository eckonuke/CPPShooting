// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerPawn.h"
#include <Particles/ParticleSystem.h>

// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//boxComponent 만들기
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	//boxComponent 크기 조정
	boxComp->SetBoxExtent(FVector(50));
	//boxComponent를 Root로 변경
	SetRootComponent(boxComp);

	//Collision Enable 설정
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Object Type 설정
	boxComp->SetCollisionObjectType(ECC_EngineTraceChannel3);
	//모든 Response를 Ignore로 설정한다.
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//Bullet은 Overlap으로 설정
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	//플레이어도 Overlap으로 설정
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	//KillBox도 Overlap으로 설정
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);

	/*boxComp->SetCollisionProfileName(TEXT("EnemyPreset"));*/

	//meshComponent 만들기
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//meshComponent를 box에 추가
	meshComp->SetupAttachment(boxComp);
	//Collision Presets을 No Collision으로 변경
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));

	//Setup explodeFactory
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempExplode(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempExplode.Succeeded()) {
		explodeFactory = tempExplode.Object;
	}
	//Setup explodeSound
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/StarterContent/Audio/Explosion01.Explosion01'"));
	if (tempSound.Succeeded()) {
		explodeSound = tempSound.Object;
	}
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

		//만약에 플레이어가 존재한다면
		if (player != nullptr) {
			//방향을 구한다
			dir = player->GetActorLocation() - GetActorLocation();
			//3. Normalize
			dir.Normalize();
		}
		else {
			dir = -GetActorRightVector();
		}
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

void AEnemy::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	
	//만약에 부딪힌 놈의 이름이 Bullet 포함하고 있다면
	//OtherActor->GetName().Contains(TEXT("Bullet")) || OtherActor->GetName().Contains(TEXT("Player"))
	//APlayerPawn* player = Cast<APlayerPawn>(OtherActor);
	if (OtherActor->GetName().Contains(TEXT("Bullet")) || OtherActor->GetName().Contains(TEXT("Player"))) {
		//1. 부딪힌 놈 파괴하자
		OtherActor->Destroy();
		//폭발효과 Effect를 생성한다
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeFactory, GetActorLocation(), GetActorRotation());
		//터질때 사운드 Effect 추가
		UGameplayStatics::PlaySound2D(GetWorld(), explodeSound);
	}
	//2. 나를 파괴하자
	Destroy();
}