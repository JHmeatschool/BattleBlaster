#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScreenMessage.generated.h"

class UTextBlock;

UCLASS()
class BATTLEBLASTER_API UScreenMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMessageText(const FString& Message);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageTextBlock;
};