#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

class ATank;

UCLASS()
class BATTLEBLASTER_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 700.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 2.0f;

	FTimerHandle FireRateTimerHandle;

	UPROPERTY()
	ATank* Tank;

	void CheckFireCondition();
	bool IsInFireRange();
};