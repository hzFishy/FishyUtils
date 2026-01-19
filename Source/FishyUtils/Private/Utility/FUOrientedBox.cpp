// By hzFishy - 2026 - Do whatever you want with it.


#include "Utility/FUOrientedBox.h"

#include "Draw/FUDraw.h"

namespace FU::Utils
{
	FFUOrientedBox::FFUOrientedBox()
	{
		
	}

	FFUOrientedBox::FFUOrientedBox(const AActor* Actor, bool bNonColliding, TArray<const UClass*> CustomIgnore, const FTransform* OverrideTransform)
	{
		if (!IsValid(Actor)) { return; }

		const bool bUseOverrideTransform = OverrideTransform != nullptr;
		const FTransform UsedTransform = bUseOverrideTransform ? *OverrideTransform : Actor->GetTransform();
		
		// Get the AABB in Local space (aka Object space: such as in the Blueprint viewer). You might want to cache this result as this may be costly.
		// instead of using CalculateComponentsBoundingBoxInLocalSpace directly we are using this modified version that allow us to whatever we want
		FBox Box(ForceInit);
		const FTransform& ActorToWorld = Actor->GetTransform();
		const FTransform WorldToActor = ActorToWorld.Inverse();
		Actor->ForEachComponent<UPrimitiveComponent>(false, [&](const UPrimitiveComponent* InPrimComp)
		{
			// apply filter
			if (CustomIgnore.Contains(InPrimComp->GetClass())) { return; }
			
			// Only use collidable components to find collision bounding box.
			if (InPrimComp->IsRegistered() && (bNonColliding || InPrimComp->IsCollisionEnabled()))
			{
				const FTransform ComponentToActor = InPrimComp->GetComponentTransform() * WorldToActor;
				Box += InPrimComp->CalcBounds(ComponentToActor).GetBox();
			}
		});

		
		// Get World space Location.
		Center = UsedTransform.TransformPosition(Box.GetCenter());
 
		// And World space extent
		const FVector Extent = Box.GetExtent();
		Forward = UsedTransform.TransformVector(FVector::ForwardVector * Extent.X);
		Right = UsedTransform.TransformVector(FVector::RightVector * Extent.Y);
		Up = UsedTransform.TransformVector(FVector::UpVector * Extent.Z);
 
		// Now you have an oriented bounding box represented by a Center and three extent vectors.
	}

	FFUOrientedBox::FFUOrientedBox(const UPrimitiveComponent* PrimitiveComponent, const FTransform* OverrideTransform)
	{
		if (!IsValid(PrimitiveComponent)) { return; }

		const FTransform ComponentTransform = OverrideTransform ? *OverrideTransform : PrimitiveComponent->GetComponentTransform();
		
		FBox Box(ForceInit);
		const FTransform& ActorToWorld = ComponentTransform;
		const FTransform WorldToActor = ActorToWorld.Inverse();
		const FTransform ComponentToActor = ComponentTransform * WorldToActor;
		Box = PrimitiveComponent->CalcBounds(ComponentToActor).GetBox();
 
		Center = ComponentTransform.TransformPosition(Box.GetCenter());
		const FVector Extent = Box.GetExtent();
		Forward = ComponentTransform.TransformVector(FVector::ForwardVector * Extent.X);
		Right = ComponentTransform.TransformVector(FVector::RightVector * Extent.Y);
		Up = ComponentTransform.TransformVector(FVector::UpVector * Extent.Z);
	}

	void FFUOrientedBox::DrawDebug(const UWorld* World, FColor Color, float LifeTime, float Thickness, uint8 DepthPriority)
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

		FU::Draw::DrawDebugLine(World, Corner1, Corner2, Color, LifeTime, Thickness, DepthPriority);
		FU::Draw::DrawDebugLine(World, Corner2, Corner3, Color, LifeTime, Thickness, DepthPriority);
		FU::Draw::DrawDebugLine(World, Corner3, Corner4, Color, LifeTime, Thickness, DepthPriority);
		FU::Draw::DrawDebugLine(World, Corner4, Corner1, Color, LifeTime, Thickness, DepthPriority);
 
		FU::Draw::DrawDebugLine(World, Corner5, Corner6, Color, LifeTime, Thickness, DepthPriority);
		FU::Draw::DrawDebugLine(World, Corner6, Corner7, Color, LifeTime, Thickness, DepthPriority);
		FU::Draw::DrawDebugLine(World, Corner7, Corner8, Color, LifeTime, Thickness, DepthPriority);
		FU::Draw::DrawDebugLine(World, Corner8, Corner5, Color, LifeTime, Thickness, DepthPriority);
 
		FU::Draw::DrawDebugLine(World, Corner1, Corner5, Color, LifeTime, Thickness, DepthPriority);
		FU::Draw::DrawDebugLine(World, Corner2, Corner6, Color, LifeTime, Thickness, DepthPriority);
		FU::Draw::DrawDebugLine(World, Corner3, Corner7, Color, LifeTime, Thickness, DepthPriority);
		FU::Draw::DrawDebugLine(World, Corner4, Corner8, Color, LifeTime, Thickness, DepthPriority);
	}

	void FFUOrientedBox::DrawDebugFrame(const UWorld* World, FColor Color, float Thickness, uint8 DepthPriority)
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
