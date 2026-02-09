#include "BattleBlasterGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "Tank.h"
#include "ScreenMessage.h"
#include "BattleBlasterGameInstance.h"
#include "GameFramework/Controller.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Tank = Cast<ATank>(PlayerPawn);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController && ScreenMessageClass)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready!");
		}
	}

	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerTimeout, 1.0f, true);
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds--;

	if (CountdownSeconds > 0)
	{
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
		}
	}
	else if (CountdownSeconds == 0)
	{
		FString StartMessage = TEXT("Go!"); 

		if (UBattleBlasterGameInstance* BBGameInstance = Cast<UBattleBlasterGameInstance>(GetGameInstance()))
		{
			int32 CurrentLevel = BBGameInstance->GetCurrentLevelIndex();

			if (CurrentLevel == 3)
			{
				StartMessage = TEXT("Final Go!");
			}
			else
			{
				StartMessage = FString::Printf(TEXT("Level %d Go!"), CurrentLevel);
			}
		}

		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->SetMessageText(StartMessage);
		}

		if (Tank)
		{
			Tank->SetPlayerEnabled(true);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		IsVictory = false;
		IsGameOver = true;
	}
	else if (ATower* DeadTower = Cast<ATower>(DeadActor))
	{
		DeadTower->HandleDestruction();
		TowerCount--;

		if (TowerCount <= 0)
		{
			IsVictory = true;
			IsGameOver = true;
		}
	}

	if (IsGameOver)
	{
		if (ScreenMessageWidget)
		{
			FString GameOverString = TEXT("Defeat!"); 

			if (IsVictory)
			{
				GameOverString = TEXT("Victory!");

				if (UBattleBlasterGameInstance* BBGameInstance = Cast<UBattleBlasterGameInstance>(GetGameInstance()))
				{
					if (BBGameInstance->GetCurrentLevelIndex() == 3)
					{
						GameOverString = TEXT("Mission Complete!");
					}
				}
			}

			ScreenMessageWidget->SetMessageText(GameOverString);
			ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);
		}

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	if (UBattleBlasterGameInstance* BBGameInstance = Cast<UBattleBlasterGameInstance>(GetGameInstance()))
	{
		if (IsVictory)
		{
			if (BBGameInstance->GetCurrentLevelIndex() == 3)
			{
				return;
			}

			BBGameInstance->LoadNextLevel();
		}
		else
		{
			BBGameInstance->RestartCurrentLevel();
		}
	}
}