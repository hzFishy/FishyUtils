﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


namespace FU_Utilities
{
	/**
	 * Represents an oriented bounding box.
	 * The extents are measured from the center to the maximum point along each axis.
	 */
	struct FISHYUTILS_API FFUOrientedBox
	{
		FFUOrientedBox();

		/* Make a oriented box from an actor */
		FFUOrientedBox(const AActor* Actor, bool bNonColliding, TArray<const UClass*> CustomIgnore = {}, const FTransform* OverrideTransform = nullptr);

		/**
		 * Make a oriented box from a primitive component
		 */
		FFUOrientedBox(const UPrimitiveComponent* PrimitiveComponent, const FTransform* OverrideTransform = nullptr);

		
		void DrawDebug(const UWorld* World, FColor Color, float LifeTime, float Thickness = 2, uint8 DepthPriority = 0);
		void DrawDebugFrame(const UWorld* World, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);
		void DrawDebugFrame(FPrimitiveDrawInterface* PDI, FColor Color, float Thickness = 2, uint8 DepthPriority = 0);

		
		FVector Center = FVector::ZeroVector;
		FVector Forward = FVector::ZeroVector;
		FVector Right = FVector::ZeroVector;
		FVector Up = FVector::ZeroVector;
	};
}
