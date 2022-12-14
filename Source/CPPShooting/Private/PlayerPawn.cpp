// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include <Components/BoxComponent.h>


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
	//�÷��̾ Ű�� �Է������� �׿� ���� ������ ����� �ش�
	FVector dir = FVector(0, h, v);
	//�ش� ������ Normalize�Ҽ� �ֵ��� ���ش�  
	dir.Normalize();
	
	/*Vector ���ϴ� �ٸ� ���
	FVector vhori = h * GetActorRightVector();
	FVector vvert = v * GetActorUpVector();
	FVector dir = vhori + vvert;
	*/
	
	//��� ���������� �̵��ϰ� �ʹ�
	// P = P0 + vt
	FVector p0 = GetActorLocation();
	FVector p = p0 + dir * speed * DeltaTime;
	SetActorLocation(p);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//BindAction -> ��ư�� �Է��߳� ���߳�
	//Horizontal Binding
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerPawn::MoveHorizontal);
	//Vertical Binding
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerPawn::MoveVertical);
}

//�÷��̾ Horizontal�� �Է��� ���� �޴´�
void APlayerPawn::MoveHorizontal(float value) {
	//�Էµ� ���� h�� �����Ѵ�
	h = value;
	//�Է��� �Ǵ��� �ȵǴ����� LOG���� Ȯ���Ѵ�
	UE_LOG(LogTemp, Warning, TEXT("Horizontal: %f"), value)
}

//�÷��̾ Vertical�� �Է��� ���� �޴´�
void APlayerPawn::MoveVertical(float value) {
	//�Էµ� ���� v�� �����Ѵ�
	v = value;
	//�Է��� �Ǵ��� �ȵǴ����� LOG���� Ȯ���Ѵ�
	UE_LOG(LogTemp, Warning, TEXT("Vertical: %f"), value)
}

