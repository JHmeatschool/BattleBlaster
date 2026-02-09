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
	ResetLives();
	ChangeLevel(FirstLevelIndex);
}