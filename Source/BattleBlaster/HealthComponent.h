#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ABattleBlasterGameMode;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLEBLASTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float Health = 0.0f;

	UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};