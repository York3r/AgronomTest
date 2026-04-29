// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GreenhouseTypes.h"
#include "Components/ActorComponent.h"
#include "GreenhouseComponent.generated.h"


UCLASS(ClassGroup=(Greenhouse), meta=(BlueprintSpawnableComponent))
class AGRONOMTEST_API UGreenhouseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UGreenhouseComponent();
	
	UFUNCTION(BlueprintCallable)
	void GetPlantStats(FName RowName, FPlantData& OutData, bool& bFound) const;

	UFUNCTION(BlueprintCallable)
	bool PlantAt(int32 X, int32 Y, FName PlantRowName);

	UFUNCTION(BlueprintCallable)
	void WaterCell(int32 X, int32 Y, float Amount);

	UFUNCTION(BlueprintCallable)
	void WaterArea(int32 StartX, int32 StartY, int32 Width, int32 Height, float Amount);

	UFUNCTION(BlueprintCallable)
	void SetLightCell(int32 X, int32 Y, float Amount);

	UFUNCTION(BlueprintCallable)
	void SetLightArea(int32 StartX, int32 StartY, int32 Width, int32 Height, float Amount);

	UFUNCTION(BlueprintCallable)
	void StepSimulation(int32 NumSteps = 1);
	
	UFUNCTION(BlueprintCallable)
	bool HarvestAt(int32 X, int32 Y, int32& OutYield);
	
	UFUNCTION(BlueprintCallable)
	int32 ClearDead();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetOccupiedCount() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetReadyToHarvestCount() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetAverageWaterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetPlantCountByType(FName PlantRowName) const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetProblematicCellsCount() const;

	
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Size")
    int32 GridWidth = 20;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Size")
    int32 GridHeight = 20;
	
    UPROPERTY(BlueprintReadOnly)
    TArray<FGreenhouseCell> Cells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SimulationInterval = 2.0f;

	FTimerHandle SimulationTimerHandle;

	UFUNCTION(BlueprintCallable)
	bool GetCellData(int32 X, int32 Y, FGreenhouseCell& OutCell) const;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* PlantTable;

	void StartSimulationTimer();
	void OnSimulationTimer();
    
    int32 GetIndex(int32 X, int32 Y) const;
    bool IsValidCoord(int32 X, int32 Y) const;
    const FPlantData* GetPlantData(FName RowName) const;
	void InitializeGrid();
    void ProcessCell(int32 Index);
    virtual void BeginPlay() override;
	void LogToScreen(const FString& Message, FColor Color = FColor::Cyan, float Duration = 5.0f) const;
	
};
