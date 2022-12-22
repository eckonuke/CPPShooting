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
	//boxComponent �����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	//boxComponent ũ�� ����
	boxComp->SetBoxExtent(FVector(50));
	//boxComponent�� Root�� ����
	SetRootComponent(boxComp);

	//Collision Enable ����
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Object Type ����
	boxComp->SetCollisionObjectType(ECC_EngineTraceChannel3);
	//��� Response�� Ignore�� �����Ѵ�.
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//Bullet�� Overlap���� ����
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	//�÷��̾ Overlap���� ����
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	//KillBox�� Overlap���� ����
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);

	/*boxComp->SetCollisionProfileName(TEXT("EnemyPreset"));*/

	//meshComponent �����
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//meshComponent�� box�� �߰�
	meshComp->SetupAttachment(boxComp);
	//Collision Presets�� No Collision���� ����
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

	//������ ���� �̴´� (1, 100)
	//���࿡ ������ ���� 50���� ������(50%) �÷��̾ ã�´�
	int32 rand = FMath::RandRange(1, 100);
	//1. �÷��̾ ã��
	AActor* player = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPawn::StaticClass());
	playerPawn = Cast<APlayerPawn>(player);

	if (rand < 50) {
		//2. �÷��̾ ���ϴ� ������ ������ (Ÿ�� - ��)
		//���࿡ �÷��̾ �����Ѵٸ�
		if (player != nullptr) {
			//������ ���Ѵ�
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

	//�浹 ������ �� �� ȣ�� �Ǵ� �Լ� ���
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
	//AcCPPShootingGameModeBase�� ��������
	//AGameModeBase* mode = GetWorld()->GetAuthGameMode();
	//ACPPShootingGameModeBase* currMode = Cast<ACPPShootingGameModeBase>(mode);

	//���Ӹ�带 �ٷ� �����ü� �ִ�
	ACPPShootingGameModeBase* currMode = GetWorld()->GetAuthGameMode<ACPPShootingGameModeBase>();

	//���࿡ �ε��� ���� �̸��� Bullet �����ϰ� �ִٸ�
	//ĳ������ �ؼ� Ȯ���ϴ� ����� �ִ�
	//APlayerPawn* player = Cast<APlayerPawn>(OtherActor);
	if (OtherActor->GetName().Contains(TEXT("Bullet"))) {
		//�Ѿ��� ��Ȱ��ȭ
		ABullet* bullet = Cast<ABullet>(OtherActor);
		bullet->setActive(false);
		//�Ѿ��� źâ�� �ִ´�
		bullet->onDestroyBullet.Broadcast(bullet);
		//playerPawn->mag.Add(bullet);

		//����ȿ�� Effect�� �����Ѵ�
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeFactory, GetActorLocation(), GetActorRotation());
		//������ ���� Effect �߰�
		UGameplayStatics::PlaySound2D(GetWorld(), explodeSound);

		currMode->AddScore(2);
	}
	else if (OtherActor->GetName().Contains(TEXT("Player"))) {
		//�÷��̾�� �浹�ϸ� �÷��̾ �����Ѵ�
		playerPawn->Destroy();
		//����ȿ�� Effect�� �����Ѵ�
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeFactory, GetActorLocation(), GetActorRotation());
		//������ ���� Effect �߰�
		UGameplayStatics::PlaySound2D(GetWorld(), explodeSound);

		//���� ����ȭ�� �����ش�
		currMode->ShowGameOverUI();
		//���� �Ͻ�����
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		//���콺 Ŀ���� �����ش�
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}
	//2. ���� �ı�����
	Destroy();
}