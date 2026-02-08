#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "InputActionValue.h"
#include "Tank.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class BATTLEBLASTER_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();
	void SetPlayerEnabled(bool bPlayerEnabled);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* TurnAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 60.0f;

	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);

	APlayerController* TankPlayerController;
	bool bIsAlive = true;
};