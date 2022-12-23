// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include <Components/BoxComponent.h>
#include <Engine/StaticMesh.h>
#include <Components/ArrowComponent.h>
#include "Bullet.h"
#include <Engine/Scene.h>
#include "../CPPShootingGameModeBase.h"



// Sets default values
// 생성자
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//컴포넌트는 생성자에서 만든다
	PrimaryActorTick.bCanEverTick = true;

	//Box Collision 만들기
	//Alt+Shift+q 헤더파일을 바로 추가할수 있는 단축키
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	//Box 를 RootComponent로 만든다
	SetRootComponent(compBox);

	//compBox의 Box Extent 사이즈를 (50, 50, 50) 으로 조절한다
	compBox->SetBoxExtent(FVector(50, 50, 50));//FVector(50)으로 해도 x,y,z에 같은 값이 알아서 지정된다
	//Collision Preset을 PlayerPreset으로 하자
	compBox->SetCollisionProfileName(TEXT("PlayerPreset"));

	//Static Mesh 만들기
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//RootComponent 자식으로 만든다
	compMesh->SetupAttachment(compBox);//compBox 대신에 RootComponent 를 사용해도 된다. 이미 compBox가 RootComponent로 선언되었기 때문에
	//StaticMesh를 세팅하자
	//Mesh의 object를 경로에 있는 object로 설정
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));//경로를 지정해야한다
	if (tempMesh.Succeeded()) {//경로에 있는 파일을 읽어오는데 성공했니?
		compMesh->SetStaticMesh(tempMesh.Object);//성공했다면 StaticMesh의 Object를 경로에 있는 object로 설정
	}
	compMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < bulletFirstCount; i++) {
		//총알을 생성한다
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, GetActorLocation(), GetActorRotation());
		//생성된 총알을 비활성화 하자
		bullet->setActive(false);
		//생성된 총알을 mag에 추가한다
		mag.Add(bullet);

		//파괴될 때 호출할 수 있는 함수 등록(델리게이트 이용!)
		bullet->onDestroyBullet.AddDynamic(this, &APlayerPawn::AddBullet);
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//플레이어가 키를 입력했을때 그에 대한 방향을 만들어 준다
	FVector dir = FVector(0, h, v);
	//해당 방향이 Normalize할수 있도록 해준다  
	dir.Normalize();

	/*
	Vector 구하는 다른 방법
	FVector vhori = h * GetActorRightVector();
	FVector vvert = v * GetActorUpVector();
	FVector dir = vhori + vvert;
	*/

	//계속 오른쪽으로 이동하고 싶다
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
	//BindAction -> 버튼을 입력했냐 안했냐
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

//플레이어가 Horizontal에 입력한 값을 받는다
void APlayerPawn::MoveHorizontal(float value) {
	//입력된 값을 h에 저장한다
	h = value;
	////입력이 되는지 안되는지를 LOG에서 확인한다
	//UE_LOG(LogTemp, Warning, TEXT("Horizontal: %f"), value)
}

//플레이어가 Vertical에 입력한 값을 받는다
void APlayerPawn::MoveVertical(float value) {
	//입력된 값을 v에 저장한다
	v = value;
	////입력이 되는지 안되는지를 LOG에서 확인한다
	//UE_LOG(LogTemp, Warning, TEXT("Vertical: %f"), value)
}

/*
* 1. 스페이스바를 누르면
* 2. 총알공장에서 총알을 만든다
* 3. 위치, 회전값을 세팅한다
*/
void APlayerPawn::InputFire() {
	//총알을 발사

	//1. 위치를 지정
	//2. 활성화
	// 만약에 mag의 갯수가 0보다 클 때
	if (mag.Num() > 0) {
		//총알의 위치, 회전 값을 Player 값으로 세팅한다
		mag[0]->SetActorLocation(GetActorLocation());
		mag[0]->SetActorRotation(GetActorRotation());
		//탄창에서 하나씩 빼서 총알을 활성화 시킨다
		mag[0]->setActive(true);
		//탄창에서 뺀다
		mag.RemoveAt(0);
	}
	else {
		//총알공장에서 총알을 만든다
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, GetActorLocation(), GetActorRotation());
		//파괴될 때 호출할 수 있는 함수 등록(델리게잍 이용!)
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
	//총알을 발사
	if (mag.Num() > 0) {
		//총알의 위치, 회전 값을 Player 값으로 세팅한다
		mag[0]->SetActorLocation(pos);
		mag[0]->SetActorRotation(rot);
		//탄창에서 하나씩 빼서 총알을 활성화 시킨다
		mag[0]->setActive(true);
		//탄창에서 뺀다
		mag.RemoveAt(0);
	}
	else {
		//총알공장에서 총알을 만든다
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, pos, rot);
		//파괴될 때 호출할 수 있는 함수 등록(델리게잍 이용!)
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