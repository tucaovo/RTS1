// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS1UserWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"

void URTS1UserWidget::ResizeBox(int MouseStartXPosition, int MouseStartYPosition, int MousePositionX, int MousePositionY) {
	double inX = MouseStartXPosition - MousePositionX;
	double inY = MouseStartYPosition - MousePositionY;
	FVector2D Vector = FVector2D(inX, inY);
	Border->SetDesiredSizeScale(Vector);
	
}