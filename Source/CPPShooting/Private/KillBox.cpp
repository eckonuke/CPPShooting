// Fill out your copyright notice in the Description page of Project Settings.


#include "KillBox.h"
#include <Components/BoxComponent.h>

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

	OtherActor->Destroy();
}