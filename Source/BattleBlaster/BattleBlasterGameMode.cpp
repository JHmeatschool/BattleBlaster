#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "Tank.h"
#include "ScreenMessage.h"
#include "BattleBlasterGameInstance.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Tank = Cast<ATank>(PlayerPawn);

	for (AActor* TowerActor : Towers)
	{
		if (ATower* Tower = Cast<ATower>(TowerActor))
		{
			if (Tank)
			{
				Tower->Tank = Tank;
			}
		}
	}

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
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->SetMessageText("Go!");
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
			FString GameOverString = IsVictory ? "Victory!" : "Defeat!";
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
			BBGameInstance->LoadNextLevel();
		}
		else
		{
			BBGameInstance->RestartCurrentLevel();
		}
	}
}