#include "Tower.h"

#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInFireRange())
	{
		if (Tank)
		{
			RotateTurret(Tank->GetActorLocation());
		}
	}
}

void ATower::CheckFireCondition()
{
	if (Tank == nullptr)
	{
		return;
	}

	bool bIsTankAlive = !Tank->IsHidden();

	if (bIsTankAlive && IsInFireRange())
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	if (Tank)
	{
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	return false;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}