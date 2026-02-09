#include "HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "BattleBlasterGameMode.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h" 

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UHealthComponent::GetHealthPercent() const
{
	if (MaxHealth <= 0.0f) return 0.0f;
	return Health / MaxHealth;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	if (GetOwner())
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);
	}
}

void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || Health <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	if (Health <= 0.0f)
	{
		if (ABattleBlasterGameMode* BBGameMode = Cast<ABattleBlasterGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			BBGameMode->ActorDied(DamagedActor);
		}
	}
}