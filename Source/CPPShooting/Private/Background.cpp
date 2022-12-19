// Fill out your copyright notice in the Description page of Project Settings.


#include "Background.h"
#include <Components/StaticMeshComponent.h>

// Sets default values
ABackground::ABackground()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//StaticMesh�� �����Ѵ�
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//StaticMesh�� �ֻ����� ��ġ�Ѵ�
	SetRootComponent(compMesh);
	//Static Mesh�� Collision�� ���°ɷ� ����
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//StaticMesh�� Plane����� ���������Ѵ�
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));//Plane�� �ִ� ��θ� �����Ѵ�
	if (tempMesh.Succeeded()) {//��� �ҷ����⸦ �����߳� Ȯ��
		compMesh->SetStaticMesh(tempMesh.Object);
	}
	//���ֱ׸� Material ����
	ConstructorHelpers::FObjectFinder<UMaterialInterface> tempMat(TEXT("Material'/Game/Material/M_universeBG.M_universeBG'"));//BG Material�� �ִ� ��θ� �����Ѵ�
	if (tempMat.Succeeded()) {//��� �ҷ����⸦ �����߳� Ȯ��
		compMesh->SetMaterial(0, tempMat.Object);
	}
}

// Called when the game starts or when spawned
void ABackground::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABackground::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//�Ʒ��� �������� P = p0 +vt
	FVector p0 = GetActorLocation();
	FVector dir = FVector::DownVector;//FVector::UpVector
	FVector p = p0 + dir * speed * DeltaTime;
	//���࿡ ��ġZ���� -3000���� �۾����� (100(Plane�⺻ũ��) * 30(scale) = 3000)
	if (p.Z < -3000) {
		p.Z += 6000;
		//SetActorLocation(FVector(0, 0, 3000));
	}
	//���� ��ġ�� ���� +3000���� ���� �ϰڴ�
	SetActorLocation(p);
}