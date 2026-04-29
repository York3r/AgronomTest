// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenhouseActor.h"

#include "GreenhouseComponent.h"


// Sets default values
AGreenhouseActor::AGreenhouseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GreenhouseComp = CreateDefaultSubobject<UGreenhouseComponent>(TEXT("GreenhouseComp"));
}

// Called when the game starts or when spawned
void AGreenhouseActor::BeginPlay()
{
	Super::BeginPlay();
	
}



