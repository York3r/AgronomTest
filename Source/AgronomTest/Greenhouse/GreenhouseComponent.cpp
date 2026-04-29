// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenhouseComponent.h"


UGreenhouseComponent::UGreenhouseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGreenhouseComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeGrid();
	StartSimulationTimer();
}

void UGreenhouseComponent::InitializeGrid()
{

	const int32 TotalCells = GridWidth * GridHeight;
	Cells.Empty();
	Cells.SetNum(TotalCells);

	for (FGreenhouseCell& Cell : Cells)
	{
		Cell = FGreenhouseCell();
	}
}


// Returns cell data from an array
bool UGreenhouseComponent::GetCellData(int32 X, int32 Y, FGreenhouseCell& OutCell) const
{
	if (!IsValidCoord(X, Y)) return false;
	OutCell = Cells[GetIndex(X, Y)];
	return true;
}

// Converts X,Y into cell array index
int32 UGreenhouseComponent::GetIndex(int32 X, int32 Y) const
{
	return Y * GridWidth + X;
}

bool UGreenhouseComponent::IsValidCoord(int32 X, int32 Y) const
{
	return X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight;
}

// Returns PlantData for internal usage
const FPlantData* UGreenhouseComponent::GetPlantData(FName RowName) const
{
	if (!PlantTable || RowName.IsNone()) return nullptr;
	return PlantTable->FindRow<FPlantData>(RowName, TEXT("GetPlantConfig"));
}

// Returns PlantData row for blueprint usage
void UGreenhouseComponent::GetPlantStats(FName RowName, FPlantData& OutData, bool& bFound) const
{
	const FPlantData* Config = GetPlantData(RowName);
	if (Config)
	{
		OutData = *Config;
		bFound = true;
	}
	else
	{
		OutData = FPlantData();
		bFound = false;
	}
}

void UGreenhouseComponent::LogToScreen(const FString& Message, FColor Color, float Duration) const
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
}

bool UGreenhouseComponent::PlantAt(int32 X, int32 Y, FName PlantRowName)
{
	if (!IsValidCoord(X, Y) || !PlantTable || !GetPlantData(PlantRowName))
	{
		LogToScreen(FString::Printf(TEXT("Failed to plant %s at [%d, %d]"),
			*PlantRowName.ToString(), X + 1, Y + 1), FColor::Yellow);
		return false;
	}
	
	const FPlantData* Config = GetPlantData(PlantRowName);
	
	FGreenhouseCell& Cell = Cells[GetIndex(X, Y)];
	
	if (!Cell.PlantRowName.IsNone() && Cell.Stage != EPlantStages::Dead)
	{
		LogToScreen(FString::Printf(TEXT("Failed to plant %s at [%d, %d] - cell occupied"),
 *PlantRowName.ToString(), X + 1, Y + 1), FColor::Yellow);
		return false;
	}
	
	Cell.PlantRowName = PlantRowName;
	Cell.Stage = EPlantStages::Seed;
	Cell.GrowthProgress = 0.0f;
	Cell.PlantHealth = Config->InitialHealth;
	
	LogToScreen(FString::Printf(TEXT("Planted %s at [%d, %d]"),
		*PlantRowName.ToString(), X+1, Y+1), FColor::Green);
	return true;
}

void UGreenhouseComponent::WaterCell(int32 X, int32 Y, float Amount)
{
	if (!IsValidCoord(X, Y))
	{
		LogToScreen(FString::Printf(TEXT("Failed to water [%d, %d] - invalid coordinates"),
			X+1, Y+1), FColor::Yellow);
		return;
	}

	FGreenhouseCell& Cell = Cells[GetIndex(X, Y)];
	Cell.WaterLevel = FMath::Clamp(Cell.WaterLevel + Amount, 0.0f, 100.0f);
	LogToScreen(FString::Printf(TEXT("Watered [%d, %d] for +%.0f water level"),
		X+1, Y+1, Amount), FColor::Blue);
}

void UGreenhouseComponent::WaterArea(int32 StartX, int32 StartY, int32 Width, int32 Height, float Amount)
{
	if (!IsValidCoord(StartX, StartY) || Width <= 0 || Height <= 0 || Amount < 0)
	{
		LogToScreen(FString::Printf(TEXT("Failed watering area [%d,%d] with range %dx%d for +%.0f water level"),
			StartX+1, StartY+1, Width, Height, Amount), FColor::Blue, 10);
		return;
	}
	for (int32 Y = StartY; Y < FMath::Min(StartY + Height, GridHeight); Y++)
	{
		for (int32 X = StartX; X < FMath::Min(StartX + Width, GridWidth); X++)
		{
			FGreenhouseCell& Cell = Cells[GetIndex(X, Y)];
			Cell.WaterLevel = FMath::Clamp(Cell.WaterLevel + Amount, 0.0f, 100.0f);
		}
	}
	LogToScreen(FString::Printf(TEXT("Watered area [%d,%d] with range %dx%d for +%.0f water level"),
		StartX+1, StartY+1, Width, Height, Amount), FColor::Blue, 10);
}

void UGreenhouseComponent::SetLightCell(int32 X, int32 Y, float Amount)
{
	if (!IsValidCoord(X, Y))
	{
		LogToScreen(FString::Printf(TEXT("Failed to set light [%d, %d] - invalid coordinates"),
			X+1, Y+1), FColor::Yellow);
		return;
	}

	FGreenhouseCell& Cell = Cells[GetIndex(X, Y)];
	Cell.LightLevel = FMath::Clamp(Cell.LightLevel + Amount, 0.0f, 100.0f);
	LogToScreen(FString::Printf(TEXT("Lit [%d, %d] for +%.0f light level"),
		X+1, Y+1, Amount), FColor::Yellow);
}

void UGreenhouseComponent::SetLightArea(int32 StartX, int32 StartY, int32 Width, int32 Height, float Amount)
{
	if (!IsValidCoord(StartX, StartY) || Width <= 0 || Height <= 0 || Amount < 0)
	{
		LogToScreen(FString::Printf(TEXT("Failed lighting area [%d,%d] with range %dx%d for +%.0f light level"),
			StartX+1, StartY+1, Width, Height, Amount), FColor::Yellow, 10);
		return;
	}
	for (int32 Y = StartY; Y < FMath::Min(StartY + Height, GridHeight); Y++)
	{
		for (int32 X = StartX; X < FMath::Min(StartX + Width, GridWidth); X++)
		{
			FGreenhouseCell& Cell = Cells[GetIndex(X, Y)];
			Cell.LightLevel = FMath::Clamp(Cell.LightLevel + Amount, 0.0f, 100.0f);
		}
	}
	LogToScreen(FString::Printf(TEXT("Lit area [%d,%d] with range %dx%d for +%.0f light level"),
		StartX + 1, StartY + 1, Width, Height, Amount), FColor::Yellow, 10);
}

void UGreenhouseComponent::StartSimulationTimer()
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	World->GetTimerManager().SetTimer(
		SimulationTimerHandle,
		this,
		&UGreenhouseComponent::OnSimulationTimer,
		SimulationInterval,
		true
	);

	LogToScreen(TEXT("Simulation timer started"));
}

void UGreenhouseComponent::OnSimulationTimer()
{
	StepSimulation(1);
}

void UGreenhouseComponent::StepSimulation(int32 NumSteps)
{
	for (int32 Step = 0; Step < NumSteps; Step++)
	{
		for (int32 Index = 0; Index < Cells.Num(); ++Index)
		{
			ProcessCell(Index);
		}
	}
	LogToScreen(FString::Printf(TEXT("Simulation: %d step(s) of %.1fs completed"),
		NumSteps, SimulationInterval));
}

void UGreenhouseComponent::ProcessCell(int32 Index)
{
    FGreenhouseCell& Cell = Cells[Index];
	
    if (Cell.PlantRowName.IsNone() || Cell.Stage == EPlantStages::Dead || Cell.Stage == EPlantStages::ReadyToHarvest)
        return;

    const FPlantData* Plant = GetPlantData(Cell.PlantRowName);
    if (!Plant) return;
	
    const bool bWaterOk = Cell.WaterLevel >= Plant->RequiredWaterMin && Cell.WaterLevel <= Plant->RequiredWaterMax;
    const bool bLightOk = Cell.LightLevel >= Plant->RequiredLightMin && Cell.LightLevel <= Plant->RequiredLightMax;

    if (bWaterOk && bLightOk)
    {
        Cell.GrowthProgress += Plant->GrowthRate;
        Cell.PlantHealth = FMath::Min(Cell.PlantHealth + Plant->HealthRegen, Plant->InitialHealth);
    }
    else
    {
        float Penalty = 0.0f;
        if (!bWaterOk) Penalty += Plant->WaterPenalty;
        if (!bLightOk) Penalty += Plant->LightPenalty;

    	Cell.PlantHealth = FMath::Max(0.0f, Cell.PlantHealth - Penalty);
    	
    }
	
    Cell.WaterLevel = FMath::Max(0.0f, Cell.WaterLevel - Plant->WaterConsumption);
	
    if (Cell.PlantHealth <= 0.0f)
    {
        Cell.Stage = EPlantStages::Dead;
        Cell.GrowthProgress = 0.0f;
        return;
    }
	
    if (Cell.GrowthProgress >= Plant->GrowthNextStage)
    {
        Cell.GrowthProgress = 0.0f;

        if (Cell.Stage == EPlantStages::Seed)
        {
            Cell.Stage = EPlantStages::Growing;
        }
        else if (Cell.Stage == EPlantStages::Growing)
        {
            Cell.Stage = EPlantStages::ReadyToHarvest;
        }
    }
}

bool UGreenhouseComponent::HarvestAt(int32 X, int32 Y, int32& OutYield)
{
	OutYield = 0;

	if (!IsValidCoord(X, Y))
	{
		LogToScreen(FString::Printf(TEXT("Failed to harvest [%d, %d] - invalid coordinates"),
			X + 1, Y + 1), FColor::Yellow);
		return false;
	}

	FGreenhouseCell& Cell = Cells[GetIndex(X, Y)];

	if (Cell.Stage != EPlantStages::ReadyToHarvest)
	{
		LogToScreen(FString::Printf(TEXT("Failed to harvest [%d, %d] - not proper stage"),
			X + 1, Y + 1), FColor::Yellow);
		return false;
	}

	const FPlantData* Config = GetPlantData(Cell.PlantRowName);
	OutYield = Config ? Config->HarvestYield : 1;

	LogToScreen(FString::Printf(TEXT("Harvested %s at [%d, %d] with +%d yield"),
		*Cell.PlantRowName.ToString(), X + 1, Y + 1, OutYield), FColor::Orange);
	
	Cell = FGreenhouseCell();

	return true;
}

int32 UGreenhouseComponent::ClearDead()
{
	int32 Count = 0;

	for (int32 Index = 0; Index < Cells.Num(); Index++)
	{
		if (Cells[Index].Stage == EPlantStages::Dead)
		{
			Cells[Index] = FGreenhouseCell();
			Count++;
		}
	}

	if (Count > 0) LogToScreen(FString::Printf(TEXT("Cleared %d dead plants"), Count), FColor::Red);


	return Count;
}

int32 UGreenhouseComponent::GetOccupiedCount() const
{
	int32 Count = 0;
	for (const FGreenhouseCell& Cell : Cells)
	{
		if (!Cell.PlantRowName.IsNone())
		{
			Count++;
		}
	}
	return Count;
}

int32 UGreenhouseComponent::GetReadyToHarvestCount() const
{
	int32 Count = 0;
	for (const FGreenhouseCell& Cell : Cells)
	{
		if (Cell.Stage == EPlantStages::ReadyToHarvest)
		{
			Count++;
		}
	}
	return Count;
}

float UGreenhouseComponent::GetAverageWaterLevel() const
{
	float Sum = 0.0f;
	int32 Count = 0;
	for (const FGreenhouseCell& Cell : Cells)
	{
		if (!Cell.PlantRowName.IsNone())
		{
			Sum += Cell.WaterLevel;
			Count++;
		}
	}
	return Count > 0 ? Sum / Count : 0.0f;
}

int32 UGreenhouseComponent::GetPlantCountByType(FName PlantRowName) const
{
	int32 Count = 0;
	for (const FGreenhouseCell& Cell : Cells)
	{
		if (Cell.PlantRowName == PlantRowName && Cell.Stage != EPlantStages::Dead)
		{
			Count++;
		}
	}
	return Count;
}

int32 UGreenhouseComponent::GetProblematicCellsCount() const
{
	int32 Count = 0;
	for (const FGreenhouseCell& Cell : Cells)
	{
		if (Cell.PlantRowName.IsNone() || Cell.Stage == EPlantStages::Dead)
			continue;

		const FPlantData* Config = GetPlantData(Cell.PlantRowName);
		if (!Config) continue;

		const bool bWaterProblem = Cell.WaterLevel < Config->RequiredWaterMin || Cell.WaterLevel > Config->RequiredWaterMax;
		const bool bLightProblem = Cell.LightLevel < Config->RequiredLightMin || Cell.LightLevel > Config->RequiredLightMax;

		if (bWaterProblem || bLightProblem)
		{
			Count++;
		}
	}
	return Count;
}

