// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include <Components/BoxComponent.h>
#include <Engine/StaticMesh.h>
#include <Components/ArrowComponent.h>
#include "Bullet.h"
#include <Engine/Scene.h>
#include "../CPPShootingGameModeBase.h"



// Sets default values
// ������
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//������Ʈ�� �����ڿ��� �����
	PrimaryActorTick.bCanEverTick = true;

	//Box Collision �����
	//Alt+Shift+q ��������� �ٷ� �߰��Ҽ� �ִ� ����Ű
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	//Box �� RootComponent�� �����
	SetRootComponent(compBox);

	//compBox�� Box Extent ����� (50, 50, 50) ���� �����Ѵ�
	compBox->SetBoxExtent(FVector(50, 50, 50));//FVector(50)���� �ص� x,y,z�� ���� ���� �˾Ƽ� �����ȴ�
	//Collision Preset�� PlayerPreset���� ����
	compBox->SetCollisionProfileName(TEXT("PlayerPreset"));

	//Static Mesh �����
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//RootComponent �ڽ����� �����
	compMesh->SetupAttachment(compBox);//compBox ��ſ� RootComponent �� ����ص� �ȴ�. �̹� compBox�� RootComponent�� ����Ǿ��� ������
	//StaticMesh�� ��������
	//Mesh�� object�� ��ο� �ִ� object�� ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));//��θ� �����ؾ��Ѵ�
	if (tempMesh.Succeeded()) {//��ο� �ִ� ������ �о���µ� �����ߴ�?
		compMesh->SetStaticMesh(tempMesh.Object);//�����ߴٸ� StaticMesh�� Object�� ��ο� �ִ� object�� ����
	}
	compMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < bulletFirstCount; i++) {
		//�Ѿ��� �����Ѵ�
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, GetActorLocation(), GetActorRotation());
		//������ �Ѿ��� ��Ȱ��ȭ ����
		bullet->setActive(false);
		//������ �Ѿ��� mag�� �߰��Ѵ�
		mag.Add(bullet);

		//�ı��� �� ȣ���� �� �ִ� �Լ� ���(��������Ʈ �̿�!)
		bullet->onDestroyBullet.AddDynamic(this, &APlayerPawn::AddBullet);
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//�÷��̾ Ű�� �Է������� �׿� ���� ������ ����� �ش�
	FVector dir = FVector(0, h, v);
	//�ش� ������ Normalize�Ҽ� �ֵ��� ���ش�  
	dir.Normalize();

	/*
	Vector ���ϴ� �ٸ� ���
	FVector vhori = h * GetActorRightVector();
	FVector vvert = v * GetActorUpVector();
	FVector dir = vhori + vvert;
	*/

	//��� ���������� �̵��ϰ� �ʹ�
	// P = P0 + vt
	FVector p0 = GetActorLocation();
	FVector p = p0 + dir * speed * DeltaTime;//dir.GetSafeNormal()

	SetActorLocation(p);

	if (FireCount < 8) {
		currTime += DeltaTime;
		if (currTime >= delayTime) {
			//InputFire();
			MakeBullet(GetActorLocation(), FRotator(0, 0, 45 * FireCount));
			FireCount++;
			currTime = 0;
		}
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//BindAction -> ��ư�� �Է��߳� ���߳�
	//Horizontal Binding
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerPawn::MoveHorizontal);
	//Vertical Binding
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayerPawn::MoveVertical);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerPawn::InputFire);
	PlayerInputComponent->BindAction(TEXT("Skill1"), IE_Pressed, this, &APlayerPawn::InputSkill);
	PlayerInputComponent->BindAction(TEXT("Skill2"), IE_Pressed, this, &APlayerPawn::InputSkill2);
	
	/*DECLARE_DELEGATE_OneParam(FTestDel, int32);
	PlayerInputComponent->BindAction<FTestDel>(TEXT("Skill2"), IE_Pressed, this, &APlayerPawn::Test, 10);*/
}

//�÷��̾ Horizontal�� �Է��� ���� �޴´�
void APlayerPawn::MoveHorizontal(float value) {
	//�Էµ� ���� h�� �����Ѵ�
	h = value;
	////�Է��� �Ǵ��� �ȵǴ����� LOG���� Ȯ���Ѵ�
	//UE_LOG(LogTemp, Warning, TEXT("Horizontal: %f"), value)
}

//�÷��̾ Vertical�� �Է��� ���� �޴´�
void APlayerPawn::MoveVertical(float value) {
	//�Էµ� ���� v�� �����Ѵ�
	v = value;
	////�Է��� �Ǵ��� �ȵǴ����� LOG���� Ȯ���Ѵ�
	//UE_LOG(LogTemp, Warning, TEXT("Vertical: %f"), value)
}

/*
* 1. �����̽��ٸ� ������
* 2. �Ѿ˰��忡�� �Ѿ��� �����
* 3. ��ġ, ȸ������ �����Ѵ�
*/
void APlayerPawn::InputFire() {
	//�Ѿ��� �߻�

	//1. ��ġ�� ����
	//2. Ȱ��ȭ
	// ���࿡ mag�� ������ 0���� Ŭ ��
	if (mag.Num() > 0) {
		//�Ѿ��� ��ġ, ȸ�� ���� Player ������ �����Ѵ�
		mag[0]->SetActorLocation(GetActorLocation());
		mag[0]->SetActorRotation(GetActorRotation());
		//źâ���� �ϳ��� ���� �Ѿ��� Ȱ��ȭ ��Ų��
		mag[0]->setActive(true);
		//źâ���� ����
		mag.RemoveAt(0);
	}
	else {
		//�Ѿ˰��忡�� �Ѿ��� �����
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, GetActorLocation(), GetActorRotation());
		//�ı��� �� ȣ���� �� �ִ� �Լ� ���(�����ԟ� �̿�!)
		bullet->onDestroyBullet.AddDynamic(this, &APlayerPawn::AddBullet);
	}
}

void APlayerPawn::InputSkill() {
	ACPPShootingGameModeBase* currMode = GetWorld()->GetAuthGameMode<ACPPShootingGameModeBase>();
	int32 count = currMode->currScore / 5 + 1;
	FVector bulletPos;
	bulletCount = count;
	for (int i = 0; i < bulletCount; i++) {
		bulletPos = GetActorLocation();
		bulletPos.Y += (i * bulletGap) - (bulletCount - 1) * (bulletGap / 2);
		MakeBullet(bulletPos, GetActorRotation());
	}
}

void APlayerPawn::InputSkill2() {
	FireCount = 0;
	return;
	FRotator rotation = GetActorRotation();
	FVector position = GetActorLocation();
	int32 count = 8, angle = 360 / count;
	for (int i = 0; i < count; i++) {
		rotation.Roll = i * angle;
		MakeBullet(position, rotation);
	}
}

void APlayerPawn::MakeBullet(FVector pos, FRotator rot) {
	//�Ѿ��� �߻�
	if (mag.Num() > 0) {
		//�Ѿ��� ��ġ, ȸ�� ���� Player ������ �����Ѵ�
		mag[0]->SetActorLocation(pos);
		mag[0]->SetActorRotation(rot);
		//źâ���� �ϳ��� ���� �Ѿ��� Ȱ��ȭ ��Ų��
		mag[0]->setActive(true);
		//źâ���� ����
		mag.RemoveAt(0);
	}
	else {
		//�Ѿ˰��忡�� �Ѿ��� �����
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, pos, rot);
		//�ı��� �� ȣ���� �� �ִ� �Լ� ���(�����ԟ� �̿�!)
		bullet->onDestroyBullet.AddDynamic(this, &APlayerPawn::AddBullet);
	}
}

void APlayerPawn::AddBullet(class ABullet* bullet) {
	UE_LOG(LogTemp, Warning, TEXT("AddBullet"));
	mag.Add(bullet);
}

void APlayerPawn::Test(int32 num)
{
	UE_LOG(LogTemp, Warning, TEXT("APlayerPawn::Test -- %d"), num);
}