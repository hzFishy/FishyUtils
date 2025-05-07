// Fill out your copyright notice in the Description page of Project Settings.


#include "Draw/FUDraw.h"

#include "Components/CapsuleComponent.h"

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
	// For some reasons if you try to draw debug an arrow/line with a depth priority above 1 for more than a frame (Time > 0) it will only be drawn for a single frame
	// As a temporary fix we reset the DepthPriority to 0 so the drawing lasts as much as requested 
	const bool bDetectTimeDepthBug = DepthPriority > 0 && Time > 0;
	if (bDetectTimeDepthBug)
	{
		DepthPriority = 0;
	}
	
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

	// For some reasons if you try to draw debug an arrow/line with a depth priority above 1 for more than a frame (Time > 0) it will only be drawn for a single frame
	// As a temporary fix we reset the DepthPriority to 0 so the drawing lasts as much as requested 
	const bool bDetectTimeDepthBug = DepthPriority > 0 && Time > 0;
	if (bDetectTimeDepthBug)
	{
		DepthPriority = 0;
	}
	
	DrawDebugDirectionalArrow(World, StartLcation, EndLocation, ArrowSize, Color, false, Time, DepthPriority, Thickness);
}
void FU_Draw::DrawDebugDirectionalArrowFrame(UWorld* World, const FVector& StartLcation, const FVector& ScaledDirection, FColor Color, float ArrowSize, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugDirectionalArrow(World, StartLcation, ScaledDirection, Color, 0, ArrowSize, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugActorBounds(UWorld* World, AActor* Actor, bool bOnlyCollidingComponents, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	FVector BoxOrigin;
	FVector BoxExtent;
	Actor->GetActorBounds(bOnlyCollidingComponents, BoxOrigin, BoxExtent, false);

	FU_Draw::DrawDebugBox(World, BoxOrigin, BoxExtent, Actor->GetActorRotation(), Color, Time, Thickness, DepthPriority);
}
void FU_Draw::DrawDebugActorBoundsFrame(UWorld* World, AActor* Actor, bool bOnlyCollidingComponents, FColor Color, float Thickness, uint8 DepthPriority)
{
	DrawDebugActorBounds(World, Actor, bOnlyCollidingComponents, Color, 0, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugCapsule(UWorld* World, const FVector& Location, const FQuat& Rotation, float Radius, float HalfHeight, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	DrawDebugCapsule(World, Location, HalfHeight, Radius, Rotation, Color, false, Time, DepthPriority, Thickness);
}
void FU_Draw::DrawDebugCapsuleFrame(UWorld* World, const FVector& Location, const FQuat& Rotation, float Radius, float HalfHeight, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugCapsule(World, Location, Rotation, Radius, HalfHeight, Color, 0, Thickness, DepthPriority);
}
void FU_Draw::DrawDebugCapsule(UWorld* World, UCapsuleComponent* CapsuleComponent, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugCapsule(World, CapsuleComponent->GetComponentLocation(), CapsuleComponent->GetComponentRotation().Quaternion(), CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight(), Color, Time, Thickness, DepthPriority);
}
void FU_Draw::DrawDebugCapsuleFrame(UWorld* World, UCapsuleComponent* CapsuleComponent, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugCapsule(World, CapsuleComponent, Color,  0, Thickness, DepthPriority);
}

void FU_Draw::DrawDebugBodyInstance(UWorld* World, const FBodyInstance& BodyInstance, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	const FBox BoxBounds = BodyInstance.GetBodyBounds();
	const FBox LocalBoxBounds = BodyInstance.GetBodyBoundsLocal();
	
	FU_Draw::DrawDebugBox(
		World,
		BoxBounds.GetCenter(),
		LocalBoxBounds.GetExtent(),
		BodyInstance.GetUnrealWorldTransform().GetRotation().Rotator(),
		Color,
		Time,
		Thickness, DepthPriority
	);
}

void FU_Draw::DrawDebugBodyInstanceFrame(UWorld* World, const FBodyInstance& BodyInstance, FColor Color, float Thickness, uint8 DepthPriority)
{
	DrawDebugBodyInstance(World, BodyInstance, Color, 0, Thickness, DepthPriority);
}
