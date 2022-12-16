// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include <Components/BoxComponent.h>
#include <Engine/StaticMesh.h>
#include <Components/ArrowComponent.h>
#include "Bullet.h"
#include <Engine/Scene.h>



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
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, GetActorLocation(), GetActorRotation());
	/*bullet->SetActorLocation(GetActorLocation());
	bullet->SetActorRotation(GetActorRotation());*/
}

