// By hzFishy 2025 - Do whatever you want with it


#include "Draw/FUDraw.h"
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Utility/FUOrientedBox.h"


void FU::Draw::DrawDebugSphere(const UWorld* World, const FVector& Location, float Radius, FColor Color, float Time,
	float Thickness, uint8 DepthPriority)
{
	DrawDebugSphere(World, Location, Radius, 10, Color, false, Time, DepthPriority, Thickness);
}
void FU::Draw::DrawDebugSphereFrame(const UWorld* World, const FVector& Location, float Radius, FColor Color,
	float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugSphere(World, Location, Radius, Color, 0, Thickness, DepthPriority);
}


void FU::Draw::DrawDebugBox(const UWorld* World, const FVector& Location, const FVector& Extent,
	const FQuat& Rotation, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	DrawDebugBox(World, Location, Extent, Rotation, Color, false, Time, DepthPriority, Thickness);
}
void FU::Draw::DrawDebugBoxFrame(const UWorld* World, const FVector& Location, const FVector& Extent,
	const FQuat& Rotation, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugBox(World, Location, Extent, Rotation, Color, 0, Thickness, DepthPriority);
}

void FU::Draw::DrawDebugBox(const UWorld* World, const FVector& Location, float HalfSize, const FQuat& Rotation,
	FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugBox(World, Location, FVector(HalfSize), Rotation, Color, Time, DepthPriority, Thickness);
}
void FU::Draw::DrawDebugBoxFrame(const UWorld* World, const FVector& Location, float HalfSize, const FQuat& Rotation,
	FColor Color, float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugBox(World, Location, HalfSize, Rotation, Color, 0, Thickness, DepthPriority);
}


void FU::Draw::DrawDebugLine(const UWorld* World, const FVector& StartLocation, const FVector& EndLocation,
	FColor Color, float Time, float Thickness, uint8 DepthPriority)
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
void FU::Draw::DrawDebugLineFrame(const UWorld* World, const FVector& StartLocation, const FVector& EndLocation,
	FColor Color, float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugLine(World, StartLocation, EndLocation, Color, 0, Thickness, DepthPriority);
}


void FU::Draw::DrawDebugLineWithMiddleText(const UWorld* World, float Time, const FVector& LineStartLocation,
	const FVector& LineEndLocation, FColor LineColor, const FString& Text, FColor TextColor, float LineThickness,
	float FontScale, uint8 LineDepthPriority)
{
	FU::Draw::DrawDebugLine(World, LineStartLocation, LineEndLocation, LineColor, Time, LineThickness, LineDepthPriority);

	const FVector MiddleLocation = LineStartLocation + (LineEndLocation - LineStartLocation)/2;
	FU::Draw::DrawDebugString(World, MiddleLocation, Text, TextColor, Time, FontScale);
}
void FU::Draw::DrawDebugLineWithMiddleTextFrame(const UWorld* World, const FVector& LineStartLocation,
	const FVector& LineEndLocation, FColor LineColor, const FString& Text, FColor TextColor, float LineThickness,
	float FontScale, uint8 LineDepthPriority)
{
	FU::Draw::DrawDebugLineWithMiddleText(World, 0, LineStartLocation, LineEndLocation, LineColor, Text, TextColor, LineThickness, FontScale, LineDepthPriority);
}


void FU::Draw::DrawDebugString(const UWorld* World, const FVector& Location, const FString& Text,
	FColor Color, float Time, float FontScale)
{
	DrawDebugString(World, Location, Text, nullptr, Color, Time, true, FontScale);
}
void FU::Draw::DrawDebugStringFrame(const UWorld* World, const FVector& Location, const FString& Text,
	FColor Color, float FontScale)
{
	FU::Draw::DrawDebugString(World, Location, Text, Color, 0, FontScale);
}


void FU::Draw::DrawDebugDirectionalArrow(const UWorld* World, const FVector& StartLcation,
	const FVector& ScaledDirection, FColor Color, float Time, float ArrowSize, float Thickness, uint8 DepthPriority)
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
void FU::Draw::DrawDebugDirectionalArrowFrame(const UWorld* World, const FVector& StartLcation,
	const FVector& ScaledDirection, FColor Color, float ArrowSize, float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugDirectionalArrow(World, StartLcation, ScaledDirection, Color, 0, ArrowSize, Thickness, DepthPriority);
}

void FU::Draw::DrawDebugDirectionalArrowWithMiddleText(const UWorld* World, const FVector& StartLcation,
	const FVector& ScaledDirection, FColor LineColor, const FString& Text, FColor TextColor, float Time,
	float ArrowSize, float Thickness, float FontScale, uint8 DepthPriority)
{
	FU::Draw::DrawDebugDirectionalArrow(World, StartLcation, ScaledDirection, LineColor, Time, ArrowSize, Thickness, DepthPriority);

	const FVector MiddleLocation = StartLcation + ScaledDirection/2;
	FU::Draw::DrawDebugString(World, MiddleLocation, Text, TextColor, Time, FontScale);
}

void FU::Draw::DrawDebugDirectionalArrowWithMiddleTextFrame(const UWorld* World, const FVector& StartLcation,
	const FVector& ScaledDirection, FColor LineColor, const FString& Text, FColor TextColor,
	float ArrowSize, float Thickness, float FontScale, uint8 DepthPriority)
{
	FU::Draw::DrawDebugDirectionalArrowWithMiddleText(World, StartLcation, ScaledDirection, LineColor, Text, TextColor, 0, ArrowSize, Thickness, FontScale, DepthPriority);
}


void FU::Draw::DrawDebugActorBounds(const UWorld* World, const AActor* Actor, bool bOnlyCollidingComponents,
	FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	FVector BoxOrigin;
	FVector BoxExtent;
	Actor->GetActorBounds(bOnlyCollidingComponents, BoxOrigin, BoxExtent, false);

	FU::Draw::DrawDebugBox(World, BoxOrigin, BoxExtent, Actor->GetActorRotation().Quaternion(), Color, Time, Thickness, DepthPriority);
}
void FU::Draw::DrawDebugActorBoundsFrame(const UWorld* World, const AActor* Actor, bool bOnlyCollidingComponents,
	FColor Color, float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugActorBounds(World, Actor, bOnlyCollidingComponents, Color, 0, Thickness, DepthPriority);
}


void FU::Draw::DrawDebugCapsule(const UWorld* World, const FVector& Location, const FQuat& Rotation,
	float Radius, float HalfHeight, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	DrawDebugCapsule(World, Location, HalfHeight, Radius, Rotation, Color, false, Time, DepthPriority, Thickness);
}
void FU::Draw::DrawDebugCapsuleFrame(const UWorld* World, const FVector& Location, const FQuat& Rotation,
	float Radius, float HalfHeight, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugCapsule(World, Location, Rotation, Radius, HalfHeight, Color, 0, Thickness, DepthPriority);
}

void FU::Draw::DrawDebugCapsule(const UWorld* World, const UCapsuleComponent* CapsuleComponent, FColor Color,
	float Time, float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugCapsule(World, CapsuleComponent->GetComponentLocation(), CapsuleComponent->GetComponentRotation().Quaternion(), CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight(), Color, Time, Thickness, DepthPriority);
}
void FU::Draw::DrawDebugCapsuleFrame(const UWorld* World, const UCapsuleComponent* CapsuleComponent, FColor Color,
	float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugCapsule(World, CapsuleComponent, Color,  0, Thickness, DepthPriority);
}


void FU::Draw::DrawDebugBodyInstance(const UWorld* World, const FBodyInstance& BodyInstance, FColor Color,
	float Time, float Thickness, uint8 DepthPriority)
{
	const FBox BoxBounds = BodyInstance.GetBodyBounds();
	const FBox LocalBoxBounds = BodyInstance.GetBodyBoundsLocal();
	
	FU::Draw::DrawDebugBox(
		World,
		BoxBounds.GetCenter(),
		LocalBoxBounds.GetExtent(),
		BodyInstance.GetUnrealWorldTransform().GetRotation(),
		Color,
		Time,
		Thickness, DepthPriority
	);
}
void FU::Draw::DrawDebugBodyInstanceFrame(const UWorld* World, const FBodyInstance& BodyInstance, FColor Color,
	float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugBodyInstance(World, BodyInstance, Color, 0, Thickness, DepthPriority);
}


void FU::Draw::DrawDebugPlane(const UWorld* World, const FVector& Location, const FVector2D& Extents,
	FColor Color, float Time, uint8 DepthPriority, bool bDrawNormal, FColor NormalColor, float NormalArrowSize)
{
	// main code copied from engine DrawDebugSolidPlane

	FPlane Plane = FPlane(0, 0, 1, Location.Z);
	
	const FVector ClosestPtOnPlane = Location - Plane.PlaneDot(Location) * Plane;

	FVector U, V;
	Plane.FindBestAxisVectors(U, V);
	U *= Extents.Y;
	V *= Extents.X;

	TArray<FVector> Verts;
	Verts.AddUninitialized(4);
	Verts[0] = ClosestPtOnPlane + U + V;
	Verts[1] = ClosestPtOnPlane - U + V;
	Verts[2] = ClosestPtOnPlane + U - V;
	Verts[3] = ClosestPtOnPlane - U - V;

	TArray<int32> Indices;
	Indices.AddUninitialized(6);
	Indices[0] = 0; Indices[1] = 2; Indices[2] = 1;
	Indices[3] = 1; Indices[4] = 2; Indices[5] = 3;

	// plane quad
	DrawDebugMesh(World, Verts, Indices, Color, false, Time, DepthPriority);

	// arrow indicating normal
	if (bDrawNormal)
	{
		DrawDebugDirectionalArrow(
			World,
			ClosestPtOnPlane, ClosestPtOnPlane + Plane * 16.f,
			NormalArrowSize,
			NormalColor,
			false, Time,
			DepthPriority
		);
	}
}
void FU::Draw::DrawDebugPlaneFrame(const UWorld* World, const FVector& Location,
	const FVector2D& Extents, FColor Color, uint8 DepthPriority, bool bDrawNormal, FColor NormalColor, float NormalArrowSize)
{
	FU::Draw::DrawDebugPlane(World, Location, Extents, Color, 0, DepthPriority);
}

void FU::Draw::DrawDebugPlane(const UWorld* World, const FVector& Location, float Extents,
	FColor Color, float Time, uint8 DepthPriority, bool bDrawNormal, FColor NormalColor, float NormalArrowSize)
{
	FU::Draw::DrawDebugPlane(World, Location, FVector2D(Extents, Extents), Color, Time, DepthPriority);
}
void FU::Draw::DrawDebugPlaneFrame(const UWorld* World, const FVector& Location, float Extents,
	FColor Color, uint8 DepthPriority, bool bDrawNormal, FColor NormalColor, float NormalArrowSize)
{
	FU::Draw::DrawDebugPlane(World, Location, Extents, Color, 0, DepthPriority);
}


void FU::Draw::DrawDebugPoint(const UWorld* World, const FVector& Position, float Size, FColor Color,
	float Time, uint8 DepthPriority)
{
	DrawDebugPoint(World, Position, Size, Color, false, Time, DepthPriority);
}
void FU::Draw::DrawDebugPointFrame(const UWorld* World, const FVector& Position, float Size, FColor Color,
	uint8 DepthPriority)
{
	FU::Draw::DrawDebugPoint(World, Position, Size, Color, 0, DepthPriority);
}


void FU::Draw::DrawDebugPrimitiveComponent(const UWorld* World, const UPrimitiveComponent* PrimitiveComponent,
                                           FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	if (!IsValid(PrimitiveComponent)) { return; }

	FBoxSphereBounds Bounds = PrimitiveComponent->CalcBounds(FTransform::Identity);
	FU::Draw::DrawDebugBox(
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
void FU::Draw::DrawDebugPrimitiveComponentFrame(const UWorld* World, const UPrimitiveComponent* PrimitiveComponent,
	FColor Color, float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugPrimitiveComponent(World, PrimitiveComponent, Color, 0, Thickness, DepthPriority);
}


void FU::Draw::DrawDebugCollisionShape(const UWorld* World, const FVector& Location, const FQuat& Rotation,
	const FCollisionShape& Shape, FColor Color, float Time, float Thickness, uint8 DepthPriority)
{
	if (Shape.IsSphere())
	{
		FU::Draw::DrawDebugSphere(World, Location, Shape.GetSphereRadius(), Color, Time, Thickness, DepthPriority);
	}
	else if (Shape.IsCapsule())
	{
		FU::Draw::DrawDebugCapsule(World, Location, Rotation, Shape.GetCapsuleRadius(), Shape.GetCapsuleHalfHeight(), Color, Time, Thickness, DepthPriority);
	}
	else if (Shape.IsBox())
	{
		FU::Draw::DrawDebugBox(World, Location, Shape.GetExtent(), Rotation, Color, Time, Thickness, DepthPriority);
	}
}
void FU::Draw::DrawDebugCollisionShapeFrame(const UWorld* World, const FVector& Location, const FQuat& Rotation,
	const FCollisionShape& Shape, FColor Color, float Thickness, uint8 DepthPriority)
{
	FU::Draw::DrawDebugCollisionShape(World, Location, Rotation, Shape, Color, 0, Thickness, DepthPriority);
}


void FU::Draw::DrawDebugOrientedActorPrimitiveComponents(const UWorld* World, const AActor* Actor, FColor Color,
	float Time, float Thickness, uint8 DepthPriority, TArray<UClass*>* IgnoredClasses)
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

void FU::Draw::DrawDebugOrientedActorPrimitiveComponentsFrame(const UWorld* World, const AActor* Actor, FColor Color,
	float Thickness, uint8 DepthPriority, TArray<UClass*>* IgnoredClasses)
{
	FU::Draw::DrawDebugOrientedActorPrimitiveComponents(World, Actor, Color, 0, Thickness, DepthPriority, IgnoredClasses);
}
