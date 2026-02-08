#include "ScreenMessage.h"
#include "Components/TextBlock.h"

void UScreenMessage::SetMessageText(const FString& Message)
{
	if (MessageTextBlock)
	{
		MessageTextBlock->SetText(FText::FromString(Message));
	}
}
