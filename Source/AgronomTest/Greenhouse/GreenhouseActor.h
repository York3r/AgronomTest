// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GreenhouseActor.generated.h"

class UGreenhouseComponent;

UCLASS()
class AGRONOMTEST_API AGreenhouseActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGreenhouseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGreenhouseComponent* GreenhouseComp;
};
