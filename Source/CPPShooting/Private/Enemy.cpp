// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerPawn.h"
#include <Particles/ParticleSystem.h>
#include "Bullet.h"
#include "../CPPShootingGameModeBase.h"

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
	//1. 플레이어를 찾자
	AActor* player = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPawn::StaticClass());
	playerPawn = Cast<APlayerPawn>(player);

	if (rand < 50) {
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

	//충돌 오버랩 될 때 호출 되는 함수 등록
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
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

	
}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//AcCPPShootingGameModeBase를 가져오자
	//AGameModeBase* mode = GetWorld()->GetAuthGameMode();
	//ACPPShootingGameModeBase* currMode = Cast<ACPPShootingGameModeBase>(mode);

	//게임모드를 바로 가져올수 있다
	ACPPShootingGameModeBase* currMode = GetWorld()->GetAuthGameMode<ACPPShootingGameModeBase>();

	//만약에 부딪힌 놈의 이름이 Bullet 포함하고 있다면
	//캐스팅을 해서 확인하는 방법도 있다
	//APlayerPawn* player = Cast<APlayerPawn>(OtherActor);
	if (OtherActor->GetName().Contains(TEXT("Bullet"))) {
		//총알을 비활성화
		ABullet* bullet = Cast<ABullet>(OtherActor);
		bullet->setActive(false);
		//총알을 탄창에 넣는다
		bullet->onDestroyBullet.Broadcast(bullet);
		//playerPawn->mag.Add(bullet);

		//폭발효과 Effect를 생성한다
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeFactory, GetActorLocation(), GetActorRotation());
		//터질때 사운드 Effect 추가
		UGameplayStatics::PlaySound2D(GetWorld(), explodeSound);

		currMode->AddScore(2);
	}
	else if (OtherActor->GetName().Contains(TEXT("Player"))) {
		//플레이어와 충돌하면 플레이어를 삭제한다
		playerPawn->Destroy();
		//폭발효과 Effect를 생성한다
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeFactory, GetActorLocation(), GetActorRotation());
		//터질때 사운드 Effect 추가
		UGameplayStatics::PlaySound2D(GetWorld(), explodeSound);

		//게임 종료화면 보여준다
		currMode->ShowGameOverUI();
		//게임 일시정지
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		//마우스 커서를 보여준다
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}
	//2. 나를 파괴하자
	Destroy();
}