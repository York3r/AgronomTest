#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GreenhouseTypes.generated.h"


UENUM(BlueprintType)
enum class EPlantStages : uint8
{
    Empty, // White box
    Seed, // Blue box
    Growing, // Yellow box
    ReadyToHarvest, // Green box
    Dead // Black box
};


USTRUCT(BlueprintType)
struct FPlantData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText DisplayName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Water")
    float RequiredWaterMin;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Water")
    float RequiredWaterMax;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Water")
    float WaterPenalty;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Water")
    float WaterConsumption;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Light")
    float RequiredLightMin;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Light")
    float RequiredLightMax;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Light")
    float LightPenalty;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float GrowthRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float GrowthNextStage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float InitialHealth;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float HealthRegen;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 HarvestYield;
};


USTRUCT(BlueprintType)
struct FGreenhouseCell
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FName PlantRowName = NAME_None;
    
    UPROPERTY(BlueprintReadOnly)
    float WaterLevel;  

    UPROPERTY(BlueprintReadOnly)
    float LightLevel;
    
    UPROPERTY(BlueprintReadOnly)
    float GrowthProgress;
    
    UPROPERTY(BlueprintReadOnly)
    EPlantStages Stage;
    
    UPROPERTY(BlueprintReadOnly)
    float PlantHealth;
};