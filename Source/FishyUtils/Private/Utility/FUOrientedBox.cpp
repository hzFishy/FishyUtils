// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/FUOrientedBox.h"

#include "Draw/FUDraw.h"

namespace FU_Utilities
{
	FFUOrientedBox::FFUOrientedBox()
	{
		
	}

	FFUOrientedBox::FFUOrientedBox(AActor* Actor, bool bNonColliding, FTransform OverrideTransform)
	{
		if (!IsValid(Actor)) { return; }
 
		// Get the AABB in Local space (aka Object space: such as in the Blueprint viewer). You might want to cache this result as this may be costly. 
		const FBox Box = Actor->CalculateComponentsBoundingBoxInLocalSpace(bNonColliding);
		const auto Transform = OverrideTransform.GetLocation().IsZero() ? Actor->GetTransform() : OverrideTransform;
 
		// Get World space Location.
		Center = Transform.TransformPosition(Box.GetCenter());
 
		// And World space extent
		const FVector Extent = Box.GetExtent();
		Forward = Transform.TransformVector(FVector::ForwardVector * Extent.X);
		Right = Transform.TransformVector(FVector::RightVector * Extent.Y);
		Up = Transform.TransformVector(FVector::UpVector * Extent.Z);
 
		// Now you have an oriented bounding box represented by a `Center` and three extent vectors.
	}

	void FFUOrientedBox::DrawDebug(UWorld* World, FColor Color, float LifeTime, float Thickness, uint8 DepthPriority)
	{
		const FVector ExtentsX = Right;
		const FVector ExtentsY = Up;
		const FVector ExtentsZ = Forward;
 
		const FVector Corner1 = Center + ExtentsX + ExtentsY + ExtentsZ;
		const FVector Corner2 = Center + ExtentsX - ExtentsY + ExtentsZ;
		const FVector Corner3 = Center - ExtentsX - ExtentsY + ExtentsZ;
		const FVector Corner4 = Center - ExtentsX + ExtentsY + ExtentsZ;
		const FVector Corner5 = Center + ExtentsX + ExtentsY - ExtentsZ;
		const FVector Corner6 = Center + ExtentsX - ExtentsY - ExtentsZ;
		const FVector Corner7 = Center - ExtentsX - ExtentsY - ExtentsZ;
		const FVector Corner8 = Center - ExtentsX + ExtentsY - ExtentsZ;

		FU_Draw::DrawDebugLine(World, Corner1, Corner2, Color, LifeTime, Thickness, DepthPriority);
		FU_Draw::DrawDebugLine(World, Corner2, Corner3, Color, LifeTime, Thickness, DepthPriority);
		FU_Draw::DrawDebugLine(World, Corner3, Corner4, Color, LifeTime, Thickness, DepthPriority);
		FU_Draw::DrawDebugLine(World, Corner4, Corner1, Color, LifeTime, Thickness, DepthPriority);
 
		FU_Draw::DrawDebugLine(World, Corner5, Corner6, Color, LifeTime, Thickness, DepthPriority);
		FU_Draw::DrawDebugLine(World, Corner6, Corner7, Color, LifeTime, Thickness, DepthPriority);
		FU_Draw::DrawDebugLine(World, Corner7, Corner8, Color, LifeTime, Thickness, DepthPriority);
		FU_Draw::DrawDebugLine(World, Corner8, Corner5, Color, LifeTime, Thickness, DepthPriority);
 
		FU_Draw::DrawDebugLine(World, Corner1, Corner5, Color, LifeTime, Thickness, DepthPriority);
		FU_Draw::DrawDebugLine(World, Corner2, Corner6, Color, LifeTime, Thickness, DepthPriority);
		FU_Draw::DrawDebugLine(World, Corner3, Corner7, Color, LifeTime, Thickness, DepthPriority);
		FU_Draw::DrawDebugLine(World, Corner4, Corner8, Color, LifeTime, Thickness, DepthPriority);
	}

	void FFUOrientedBox::DrawDebugFrame(UWorld* World, FColor Color, float Thickness, uint8 DepthPriority)
	{
		DrawDebug(World, Color, 0, Thickness, DepthPriority);
	}

	void FFUOrientedBox::DrawDebugFrame(FPrimitiveDrawInterface* PDI, FColor Color, float Thickness, uint8 DepthPriority)
	{
		const FVector ExtentsX = Right;
		const FVector ExtentsY = Up;
		const FVector ExtentsZ = Forward;
 
		const FVector Corner1 = Center + ExtentsX + ExtentsY + ExtentsZ;
		const FVector Corner2 = Center + ExtentsX - ExtentsY + ExtentsZ;
		const FVector Corner3 = Center - ExtentsX - ExtentsY + ExtentsZ;
		const FVector Corner4 = Center - ExtentsX + ExtentsY + ExtentsZ;
		const FVector Corner5 = Center + ExtentsX + ExtentsY - ExtentsZ;
		const FVector Corner6 = Center + ExtentsX - ExtentsY - ExtentsZ;
		const FVector Corner7 = Center - ExtentsX - ExtentsY - ExtentsZ;
		const FVector Corner8 = Center - ExtentsX + ExtentsY - ExtentsZ;
		
		PDI->DrawLine(Corner1, Corner2, Color, SDPG_World, Thickness, DepthPriority, false);
		PDI->DrawLine(Corner2, Corner3, Color, SDPG_World, Thickness, DepthPriority, false);
		PDI->DrawLine(Corner3, Corner4, Color, SDPG_World, Thickness, DepthPriority, false);
		PDI->DrawLine(Corner4, Corner1, Color, SDPG_World, Thickness, DepthPriority, false);
 
		PDI->DrawLine(Corner5, Corner6, Color, SDPG_World, Thickness, DepthPriority, false);
		PDI->DrawLine(Corner6, Corner7, Color, SDPG_World, Thickness, DepthPriority, false);
		PDI->DrawLine(Corner7, Corner8, Color, SDPG_World, Thickness, DepthPriority, false);
		PDI->DrawLine(Corner8, Corner5, Color, SDPG_World, Thickness, DepthPriority, false);
 
		PDI->DrawLine(Corner1, Corner5, Color, SDPG_World, Thickness, DepthPriority, false);
		PDI->DrawLine(Corner2, Corner6, Color, SDPG_World, Thickness, DepthPriority, false);
		PDI->DrawLine(Corner3, Corner7, Color, SDPG_World, Thickness, DepthPriority, false);
		PDI->DrawLine(Corner4, Corner8, Color, SDPG_World, Thickness, DepthPriority, false);
	}
}
