#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "Tank.h"
#include "ScreenMessage.h"
#include "BattleBlasterGameInstance.h"
#include "GameFramework/Controller.h"
#include "Blueprint/UserWidget.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Tank = Cast<ATank>(PlayerPawn);

	if (Tank)
	{
		Tank->SetPlayerEnabled(false);
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController && StartHUDClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(PlayerController, StartHUDClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

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
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();

		if (Tank->GetController())
		{
			Tank->SetPlayerEnabled(false);
		}

		if (UBattleBlasterGameInstance* BBGameInstance = Cast<UBattleBlasterGameInstance>(GetGameInstance()))
		{
			if (!BBGameInstance->ConsumeLife())
			{
				IsGameOver = true;
			}
		}

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ABattleBlasterGameMode::OnGameOverTimerTimeout);
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, GameOverDelay, false);
	}
	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();

		TowerCount--;
		if (TowerCount == 0)
		{
			IsVictory = true;

			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ABattleBlasterGameMode::OnGameOverTimerTimeout);
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, GameOverDelay, false);
		}
	}

	if (IsGameOver || IsVictory)
	{
		if (ScreenMessageWidget)
		{
			FString GameOverString = IsVictory ? TEXT("Victory!") : TEXT("Game Over!");

			if (IsVictory)
			{
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
			if (BBGameInstance->GetPlayerLives() > 0)
			{
				BBGameInstance->RestartCurrentLevel();
			}
			else
			{
				return;
			}
		}
	}
}