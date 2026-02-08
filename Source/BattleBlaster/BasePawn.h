#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class UCapsuleComponent;
class AProjectile;
class UNiagaraSystem;
class USoundBase;
class UCameraShakeBase;

UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();

	void RotateTurret(FVector LookAtTarget);
	void Fire();
	virtual void HandleDestruction();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UNiagaraSystem* DeathParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float TurretInterpSpeed = 10.0f;
};