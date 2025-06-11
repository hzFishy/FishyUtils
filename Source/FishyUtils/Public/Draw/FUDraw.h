// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UCapsuleComponent;


namespace FU_Draw
{
	FISHYUTILS_API void DrawDebugSphere(const UWorld* World, const FVector& Location, float Radius, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugSphereFrame(const UWorld* World, const FVector& Location, float Radius, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugBox(const UWorld* World, const FVector& Location, const FVector& Extent, const FQuat& Rotation, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugBoxFrame(const UWorld* World, const FVector& Location, const FVector& Extent, const FQuat& Rotation, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugBox(const UWorld* World, const FVector& Location, float HalfSize, const FQuat& Rotation, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugBoxFrame(const UWorld* World, const FVector& Location, float HalfSize, const FQuat& Rotation, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugLine(const UWorld* World, const FVector& StartLocation, const FVector& EndLocation, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugLineFrame(const UWorld* World, const FVector& StartLocation, const FVector& EndLocation, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugLineWithMiddleText(const UWorld* World, float Time, const FVector& LineStartLocation, const FVector& LineEndLocation, FColor LineColor, const FString& Text, FColor TextColor, float LineThickness = 2, float FontScale = 2, uint8 LineDepthPriority = 0);
	FISHYUTILS_API void DrawDebugLineWithMiddleTextFrame(const UWorld* World, const FVector& LineStartLocation, const FVector& LineEndLocation, FColor LineColor, const FString& Text, FColor TextColor, float LineThickness = 2, float FontScale = 2, uint8 LineDepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugString(const UWorld* World, const FVector& Location, const FString& Text, FColor Color, float Time, float FontScale = 2);
	FISHYUTILS_API void DrawDebugStringFrame(const UWorld* World, const FVector& Location, const FString& Text, FColor Color, float FontScale = 2);

	
	FISHYUTILS_API void DrawDebugDirectionalArrow(const UWorld* World, const FVector& StartLcation, const FVector& ScaledDirection, FColor Color, float Time, float ArrowSize = 2, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugDirectionalArrowFrame(const UWorld* World, const FVector& StartLcation, const FVector& ScaledDirection, FColor Color, float ArrowSize = 2, float Thickness = 2, uint8 DepthPriority = 0);

	/* For more control and precision see FFUOrientedBox */
	FISHYUTILS_API void DrawDebugActorBounds(const UWorld* World, const AActor* Actor, bool bOnlyCollidingComponents, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugActorBoundsFrame(const UWorld* World, const AActor* Actor, bool bOnlyCollidingComponents, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugCapsule(const UWorld* World, const FVector& Location, const FQuat& Rotation, float Radius, float HalfHeight, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugCapsuleFrame(const UWorld* World, const FVector& Location, const FQuat& Rotation, float Radius, float HalfHeight, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugCapsule(const UWorld* World, const UCapsuleComponent* CapsuleComponent, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugCapsuleFrame(const UWorld* World, const UCapsuleComponent* CapsuleComponent, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugBodyInstance(const UWorld* World, const FBodyInstance& BodyInstance, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugBodyInstanceFrame(const UWorld* World, const FBodyInstance& BodyInstance, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	/**
	 * Fine in most cases, for very precise draw see FFUOrientedBox.
	 * This is different then DrawDebugCollisionShape because we aren't using a precise FCollisionShape.
	 */
	FISHYUTILS_API void DrawDebugPrimitiveComponent(const UWorld* World, const UPrimitiveComponent* PrimitiveComponent, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugPrimitiveComponentFrame(const UWorld* World, const UPrimitiveComponent* PrimitiveComponent, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

	
	FISHYUTILS_API void DrawDebugCollisionShape(const UWorld* World, const FVector& Location, const FQuat& Rotation, const FCollisionShape& Shape, FColor Color, float Time, float Thickness = 2, uint8 DepthPriority = 0);
	FISHYUTILS_API void DrawDebugCollisionShapeFrame(const UWorld* World, const FVector& Location, const FQuat& Rotation, const FCollisionShape& Shape, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);
}
