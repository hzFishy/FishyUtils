// By hzFishy 2025 - Do whatever you want with it


#include "Draw/FUDraw.h"
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Utility/FUOrientedBox.h"


void FU_Draw::DrawDebugSphere(const UWorld* World, const FVector& Location, float Radius, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	DrawDebugSphere(World, Location, Radius, 10, Color, false, Time, DepthPriority, Thickness);
}
void FU_Draw::DrawDebugSphereFrame(const UWorld* World, const FVector& Location, float Radius, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugSphere(World, Location, Radius, Color, 0, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugBox(const UWorld* World, const FVector& Location, const FVector& Extent, const FQuat& Rotation, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	DrawDebugBox(World, Location, Extent, Rotation, Color, false, Time, DepthPriority, Thickness);
}
void FU_Draw::DrawDebugBoxFrame(const UWorld* World, const FVector& Location, const FVector& Extent, const FQuat& Rotation, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugBox(World, Location, Extent, Rotation, Color, 0, Thickness, DepthPriority);
}
void FU_Draw::DrawDebugBox(const UWorld* World, const FVector& Location, float HalfSize, const FQuat& Rotation, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugBox(World, Location, FVector(HalfSize), Rotation, Color, Time, DepthPriority, Thickness);
}
void FU_Draw::DrawDebugBoxFrame(const UWorld* World, const FVector& Location, float HalfSize, const FQuat& Rotation, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugBox(World, Location, HalfSize, Rotation, Color, 0, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugLine(const UWorld* World, const FVector& StartLocation, const FVector& EndLocation, FColor Color, float Time, float Thickness, uint8 DepthPriority)
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
void FU_Draw::DrawDebugLineFrame(const UWorld* World, const FVector& StartLocation, const FVector& EndLocation, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugLine(World, StartLocation, EndLocation, Color, 0, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugLineWithMiddleText(const UWorld* World, float Time, const FVector& LineStartLocation, const FVector& LineEndLocation, FColor LineColor, const FString& Text, FColor TextColor, float LineThickness, float FontScale, uint8 LineDepthPriority)
{
	FU_Draw::DrawDebugLine(World, LineStartLocation, LineEndLocation, LineColor, Time, LineThickness, LineDepthPriority);

	const FVector MiddleLocation = LineStartLocation + (LineEndLocation - LineStartLocation)/2;
	FU_Draw::DrawDebugString(World, MiddleLocation, Text, TextColor, Time, FontScale);
}
void FU_Draw::DrawDebugLineWithMiddleTextFrame(const UWorld* World, const FVector& LineStartLocation, const FVector& LineEndLocation, FColor LineColor, const FString& Text, FColor TextColor, float LineThickness, float FontScale, uint8 LineDepthPriority)
{
	FU_Draw::DrawDebugLineWithMiddleText(World, 0, LineStartLocation, LineEndLocation, LineColor, Text, TextColor, LineThickness, FontScale, LineDepthPriority);
}


void FU_Draw::DrawDebugString(const UWorld* World, const FVector& Location, const FString& Text, FColor Color, float Time, float FontScale)
{
	DrawDebugString(World, Location, Text, nullptr, Color, Time, true, FontScale);
}
void FU_Draw::DrawDebugStringFrame(const UWorld* World, const FVector& Location, const FString& Text, FColor Color, float FontScale)
{
	FU_Draw::DrawDebugString(World, Location, Text, Color, 0, FontScale);
}


void FU_Draw::DrawDebugDirectionalArrow(const UWorld* World, const FVector& StartLcation, const FVector& ScaledDirection, FColor Color, float Time, float ArrowSize, float Thickness, uint8 DepthPriority)
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
void FU_Draw::DrawDebugDirectionalArrowFrame(const UWorld* World, const FVector& StartLcation, const FVector& ScaledDirection, FColor Color, float ArrowSize, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugDirectionalArrow(World, StartLcation, ScaledDirection, Color, 0, ArrowSize, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugActorBounds(const UWorld* World, const AActor* Actor, bool bOnlyCollidingComponents, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	FVector BoxOrigin;
	FVector BoxExtent;
	Actor->GetActorBounds(bOnlyCollidingComponents, BoxOrigin, BoxExtent, false);

	FU_Draw::DrawDebugBox(World, BoxOrigin, BoxExtent, Actor->GetActorRotation().Quaternion(), Color, Time, Thickness, DepthPriority);
}
void FU_Draw::DrawDebugActorBoundsFrame(const UWorld* World, const AActor* Actor, bool bOnlyCollidingComponents, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugActorBounds(World, Actor, bOnlyCollidingComponents, Color, 0, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugCapsule(const UWorld* World, const FVector& Location, const FQuat& Rotation, float Radius, float HalfHeight, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	DrawDebugCapsule(World, Location, HalfHeight, Radius, Rotation, Color, false, Time, DepthPriority, Thickness);
}
void FU_Draw::DrawDebugCapsuleFrame(const UWorld* World, const FVector& Location, const FQuat& Rotation, float Radius, float HalfHeight, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugCapsule(World, Location, Rotation, Radius, HalfHeight, Color, 0, Thickness, DepthPriority);
}
void FU_Draw::DrawDebugCapsule(const UWorld* World, const UCapsuleComponent* CapsuleComponent, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugCapsule(World, CapsuleComponent->GetComponentLocation(), CapsuleComponent->GetComponentRotation().Quaternion(), CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight(), Color, Time, Thickness, DepthPriority);
}
void FU_Draw::DrawDebugCapsuleFrame(const UWorld* World, const UCapsuleComponent* CapsuleComponent, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugCapsule(World, CapsuleComponent, Color,  0, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugBodyInstance(const UWorld* World, const FBodyInstance& BodyInstance, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	const FBox BoxBounds = BodyInstance.GetBodyBounds();
	const FBox LocalBoxBounds = BodyInstance.GetBodyBoundsLocal();
	
	FU_Draw::DrawDebugBox(
		World,
		BoxBounds.GetCenter(),
		LocalBoxBounds.GetExtent(),
		BodyInstance.GetUnrealWorldTransform().GetRotation(),
		Color,
		Time,
		Thickness, DepthPriority
	);
}
void FU_Draw::DrawDebugBodyInstanceFrame(const UWorld* World, const FBodyInstance& BodyInstance, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugBodyInstance(World, BodyInstance, Color, 0, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugPrimitiveComponent(const UWorld* World, const UPrimitiveComponent* PrimitiveComponent, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	if (!IsValid(PrimitiveComponent)) { return; }

	FBoxSphereBounds Bounds = PrimitiveComponent->CalcBounds(FTransform::Identity);
	FU_Draw::DrawDebugBox(
		World,
		// TODO: not working with rotation
		PrimitiveComponent->GetComponentLocation() + (Bounds.Origin * PrimitiveComponent->GetComponentScale()),
		Bounds.BoxExtent,
		PrimitiveComponent->GetComponentRotation().Quaternion(),
		Color, Time,
		Thickness, DepthPriority
	);
	return;
}
void FU_Draw::DrawDebugPrimitiveComponentFrame(const UWorld* World, const UPrimitiveComponent* PrimitiveComponent, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugPrimitiveComponent(World, PrimitiveComponent, Color, 0, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugCollisionShape(const UWorld* World, const FVector& Location, const FQuat& Rotation, const FCollisionShape& Shape, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	if (Shape.IsSphere())
	{
		FU_Draw::DrawDebugSphere(World, Location, Shape.GetSphereRadius(), Color, Time, Thickness, DepthPriority);
	}
	else if (Shape.IsCapsule())
	{
		FU_Draw::DrawDebugCapsule(World, Location, Rotation, Shape.GetCapsuleRadius(), Shape.GetCapsuleHalfHeight(), Color, Time, Thickness, DepthPriority);
	}
	else if (Shape.IsBox())
	{
		FU_Draw::DrawDebugBox(World, Location, Shape.GetExtent(), Rotation, Color, Time, Thickness, DepthPriority);
	}
}
void FU_Draw::DrawDebugCollisionShapeFrame(const UWorld* World, const FVector& Location, const FQuat& Rotation, const FCollisionShape& Shape, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU_Draw::DrawDebugCollisionShape(World, Location, Rotation, Shape, Color, 0, Thickness, DepthPriority);
}


void FU_Draw::DrawDebugOrientedActorPrimitiveComponents(const UWorld* World, const AActor* Actor, FColor Color, float Time, float Thickness, uint8 DepthPriority, TArray<UClass*>* IgnoredClasses)
{
	if (!IgnoredClasses)
	{
		IgnoredClasses = new TArray<UClass*>();
	}

	IgnoredClasses->Add(UBillboardComponent::StaticClass());
	IgnoredClasses->Add(UTextRenderComponent::StaticClass());
	
	Actor->ForEachComponent<UPrimitiveComponent>(false, [&](const UPrimitiveComponent* InPrimComp)
	{
		for (auto& IgnoredClass : *IgnoredClasses)
		{
			if (InPrimComp->GetClass()->IsChildOf(IgnoredClass)) { return; }
		}
		
		FU::Utils::FFUOrientedBox OrientedBox(InPrimComp);
		OrientedBox.DrawDebug(World, Color, Time, Thickness, DepthPriority);
	});
}

void FU_Draw::DrawDebugOrientedActorPrimitiveComponentsFrame(const UWorld* World, const AActor* Actor, FColor Color, float Thickness, uint8 DepthPriority, TArray<UClass*>* IgnoredClasses)
{
	FU_Draw::DrawDebugOrientedActorPrimitiveComponents(World, Actor, Color, 0, Thickness, DepthPriority, IgnoredClasses);
}
