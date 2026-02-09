#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleBlasterGameInstance.generated.h"

UCLASS()
class BATTLEBLASTER_API UBattleBlasterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void LoadNextLevel();
	void RestartCurrentLevel();
	void RestartGame();

	int32 GetCurrentLevelIndex() const { return CurrentLevelIndex; }

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerLives() const { return PlayerLives; }

	bool ConsumeLife();

	void ResetLives();

	void AddDestroyedTower(FString TowerName);

	bool IsTowerDestroyed(FString TowerName);

	void ClearDestroyedTowers();

private:
	void ChangeLevel(int32 Index);

	UPROPERTY(EditDefaultsOnly, Category = "Level Setup")
	FString LevelNamePrefix = TEXT("Level_");

	UPROPERTY(EditDefaultsOnly, Category = "Level Setup")
	int32 FirstLevelIndex = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Level Setup")
	int32 LastLevelIndex = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Status", meta = (AllowPrivateAccess = "true"))
	int32 CurrentLevelIndex = 1;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerLives = 3;

	TArray<FString> DestroyedTowers;
};