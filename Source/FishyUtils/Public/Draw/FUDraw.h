﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UCapsuleComponent;


namespace FU_Draw
{
	FISHYUTILS_API void DrawDebugSphere(UWorld* World, const FVector& Location, float Radius, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugSphereFrame(UWorld* World, const FVector& Location, float Radius, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugBox(UWorld* World, const FVector& Location, const FVector& Extent, const FQuat& Rotation, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugBoxFrame(UWorld* World, const FVector& Location, const FVector& Extent, const FQuat& Rotation, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugBox(UWorld* World, const FVector& Location, float HalfSize, const FQuat& Rotation, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugBoxFrame(UWorld* World, const FVector& Location, float HalfSize, const FQuat& Rotation, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugLine(UWorld* World, const FVector& StartLocation, const FVector& EndLocation, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugLineFrame(UWorld* World, const FVector& StartLocation, const FVector& EndLocation, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugLineWithMiddleText(UWorld* World, float Time, const FVector& LineStartLocation, const FVector& LineEndLocation, FColor LineColor, const FString& Text, FColor TextColor, float LineThickness = 2, float FontScale = 2, uint8 LineDepthPriority = 0);
	FISHYUTILS_API void DrawDebugLineWithMiddleTextFrame(UWorld* World, const FVector& LineStartLocation, const FVector& LineEndLocation, FColor LineColor, const FString& Text, FColor TextColor, float LineThickness = 2, float FontScale = 2, uint8 LineDepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugString(UWorld* World, const FVector& Location, const FString& Text, FColor Color, float Time, float FontScale = 2);
	FISHYUTILS_API void DrawDebugStringFrame(UWorld* World, const FVector& Location, const FString& Text, FColor Color, float FontScale = 2);

	
	FISHYUTILS_API void DrawDebugDirectionalArrow(UWorld* World, const FVector& StartLcation, const FVector& ScaledDirection, FColor Color, float Time, float ArrowSize = 2, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugDirectionalArrowFrame(UWorld* World, const FVector& StartLcation, const FVector& ScaledDirection, FColor Color, float ArrowSize = 2, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugActorBounds(UWorld* World, AActor* Actor, bool bOnlyCollidingComponents, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugActorBoundsFrame(UWorld* World, AActor* Actor, bool bOnlyCollidingComponents, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugCapsule(UWorld* World, const FVector& Location, const FQuat& Rotation, float Radius, float HalfHeight, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugCapsuleFrame(UWorld* World, const FVector& Location, const FQuat& Rotation, float Radius, float HalfHeight, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugCapsule(UWorld* World, UCapsuleComponent* CapsuleComponent, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugCapsuleFrame(UWorld* World, UCapsuleComponent* CapsuleComponent, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugBodyInstance(UWorld* World, const FBodyInstance& BodyInstance, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugBodyInstanceFrame(UWorld* World, const FBodyInstance& BodyInstance, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	/* Fine in most cases, for very precise draw see FFUOrientedBox */
	FISHYUTILS_API void DrawDebugPrimitiveComponent(UWorld* World, UPrimitiveComponent* PrimitiveComponent, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugPrimitiveComponentFrame(UWorld* World, UPrimitiveComponent* PrimitiveComponent, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugCollisionShape(UWorld* World, const FVector& Location, const FQuat& Rotation, const FCollisionShape& Shape, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugCollisionShapeFrame(UWorld* World, const FVector& Location, const FQuat& Rotation, const FCollisionShape& Shape, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);
}
