// Fill out your copyright notice in the Description page of Project Settings.


#include "Draw/FUDraw.h"

void FU_Draw::DrawDebugSphere(UWorld* World, const FVector& Location, float Radius, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	DrawDebugSphere(World, Location, Radius, 10, Color, false, Time, DepthPriority, Thickness);
}
void FU_Draw::DrawDebugSphereFrame(UWorld* World, const FVector& Location, float Radius, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugSphere(World, Location, Radius, Color, 0, Thickness, DepthPriority);
}

void FU_Draw::DrawDebugBox(UWorld* World, const FVector& Location, const FVector& Extent, const FRotator& Rotation, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	DrawDebugBox(World, Location, Extent, Rotation.Quaternion(), Color, false, Time, DepthPriority, Thickness);
}
void FU_Draw::DrawDebugBoxFrame(UWorld* World, const FVector& Location, const FVector& Extent, const FRotator& Rotation, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugBox(World, Location, Extent, Rotation, Color, 0, Thickness, DepthPriority);
}
void FU_Draw::DrawDebugBox(UWorld* World, const FVector& Location, float HalfSize, const FRotator& Rotation, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugBox(World, Location, FVector(HalfSize), Rotation, Color, Time, DepthPriority, Thickness);
}
void FU_Draw::DrawDebugBoxFrame(UWorld* World, const FVector& Location, float HalfSize, const FRotator& Rotation, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugBox(World, Location, HalfSize, Rotation, Color, 0, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugLine(UWorld* World, const FVector& StartLocation, const FVector& EndLocation, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	DrawDebugLine(World, StartLocation, EndLocation, Color, false, Time, DepthPriority, Thickness);
}
void FU_Draw::DrawDebugLineFrame(UWorld* World, const FVector& StartLocation, const FVector& EndLocation, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugLine(World, StartLocation, EndLocation, Color, 0, Thickness, DepthPriority);
}

void FU_Draw::DrawDebugString(UWorld* World, const FVector& Location, const FString& Text, FColor Color, float Time, float FontScale)
{
	DrawDebugString(World, Location, Text, nullptr, Color, Time, true, FontScale);
}

void FU_Draw::DrawDebugStringFrame(UWorld* World, const FVector& Location, const FString& Text, FColor Color, float FontScale)
{
	FU_Draw::DrawDebugString(World, Location, Text, Color, 0, FontScale);
}

void FU_Draw::DrawDebugDirectionalArrow(UWorld* World, const FVector& StartLcation, const FVector& ScaledDirection, FColor Color, float Time, float ArrowSize, float Thickness, uint8 DepthPriority)
{
	const FVector EndLocation = StartLcation + ScaledDirection;
	DrawDebugDirectionalArrow(World, StartLcation, EndLocation, ArrowSize, Color, false, Time, DepthPriority, Thickness);
}

void FU_Draw::DrawDebugDirectionalArrowFrame(UWorld* World, const FVector& StartLcation, const FVector& ScaledDirection, FColor Color, float ArrowSize, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugDirectionalArrow(World, StartLcation, ScaledDirection, Color, 0, ArrowSize, Thickness, DepthPriority);
}