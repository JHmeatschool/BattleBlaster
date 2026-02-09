#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleBlasterGameMode.generated.h"

class ATank;
class UScreenMessage;

UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	void ActorDied(AActor* DeadActor);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Game Loop")
	TSubclassOf<UScreenMessage> ScreenMessageClass;

	UPROPERTY(VisibleInstanceOnly, Category = "Game Loop")
	UScreenMessage* ScreenMessageWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> StartHUDClass;

	UPROPERTY()
	class UUserWidget* HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Game Loop")
	float GameOverDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Game Loop")
	int32 CountdownDelay = 3;

	int32 CountdownSeconds = 0;
	int32 TowerCount = 0;
	bool IsVictory = false;

	FTimerHandle CountdownTimerHandle;

	UPROPERTY(VisibleInstanceOnly, Category = "Game Loop")
	ATank* Tank;

	void OnGameOverTimerTimeout();
	void OnCountdownTimerTimeout();
};