// Fill out your copyright notice in the Description page of Project Settings.


#include "Background.h"
#include <Components/StaticMeshComponent.h>

// Sets default values
ABackground::ABackground()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//StaticMesh를 생성한다
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//StaticMesh를 최상위로 위치한다
	SetRootComponent(compMesh);
	//Static Mesh의 Collision을 없는걸로 설정
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//StaticMesh가 Plane모양을 가지도록한다
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));//Plane이 있는 경로를 지정한다
	if (tempMesh.Succeeded()) {//경로 불러오기를 성공했냐 확인
		compMesh->SetStaticMesh(tempMesh.Object);
	}
	//우주그림 Material 세팅
	ConstructorHelpers::FObjectFinder<UMaterialInterface> tempMat(TEXT("Material'/Game/Material/M_universeBG.M_universeBG'"));//BG Material이 있는 경로를 지정한다
	if (tempMat.Succeeded()) {//경로 불러오기를 성공했냐 확인
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
	//아래로 움직이자 P = p0 +vt
	FVector p0 = GetActorLocation();
	FVector dir = FVector::DownVector;//FVector::UpVector
	FVector p = p0 + dir * speed * DeltaTime;
	//만약에 위치Z값이 -3000보다 작아지면 (100(Plane기본크기) * 30(scale) = 3000)
	if (p.Z < -3000) {
		p.Z += 6000;
		//SetActorLocation(FVector(0, 0, 3000));
	}
	//나의 위치를 위로 +3000으로 설정 하겠다
	SetActorLocation(p);
}