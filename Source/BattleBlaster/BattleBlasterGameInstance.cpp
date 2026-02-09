#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

bool UBattleBlasterGameInstance::ConsumeLife()
{
	if (PlayerLives <= 0)
	{
		return false;
	}

	PlayerLives--;
	return PlayerLives > 0;
}

void UBattleBlasterGameInstance::ResetLives()
{
	PlayerLives = 3;
}

void UBattleBlasterGameInstance::AddDestroyedTower(FString TowerName)
{
	DestroyedTowers.AddUnique(TowerName);
}

bool UBattleBlasterGameInstance::IsTowerDestroyed(FString TowerName)
{
	return DestroyedTowers.Contains(TowerName);
}

void UBattleBlasterGameInstance::ClearDestroyedTowers()
{
	DestroyedTowers.Empty();
}

void UBattleBlasterGameInstance::ChangeLevel(int32 Index)
{
	if (Index >= FirstLevelIndex && Index <= LastLevelIndex)
	{
		CurrentLevelIndex = Index;

		FString LevelNameString = FString::Printf(TEXT("%s%d"), *LevelNamePrefix, CurrentLevelIndex);
		UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelNameString));
	}
}

void UBattleBlasterGameInstance::LoadNextLevel()
{
	ClearDestroyedTowers();

	if (CurrentLevelIndex < LastLevelIndex)
	{
		ChangeLevel(CurrentLevelIndex + 1);
	}
	else
	{
		RestartGame();
	}
}

void UBattleBlasterGameInstance::RestartCurrentLevel()
{
	ChangeLevel(CurrentLevelIndex);
}

void UBattleBlasterGameInstance::RestartGame()
{
	ClearDestroyedTowers();
	ResetLives();
	ChangeLevel(FirstLevelIndex);
}