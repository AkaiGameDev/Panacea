// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionHintWidget.h"

void UInteractionHintWidget::SetTextOfInteractionHint(const FText InText)
{
	InteractionHintText = InText;
}

FText UInteractionHintWidget::GetTextOfInteractionHint() const
{
	return InteractionHintText;
}
