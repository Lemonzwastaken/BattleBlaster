// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString Message)
{
    if (MessageTextBlock)
    {
        MessageTextBlock->SetText(FText::FromString(Message));
    }
}

void UScreenMessage::SetComboText(FString ComboMessage)
{
    if (ComboMessageBlock)
    {
        ComboMessageBlock->SetText(FText::FromString(ComboMessage));
    }
}