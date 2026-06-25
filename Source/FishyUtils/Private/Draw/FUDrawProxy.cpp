// By hzFishy - 2026 - Do whatever you want with it.


#include "Draw/FUDrawProxy.h"

#include "Engine/Canvas.h"

inline const TCHAR* TargetShowFlagName = TEXT("FU_Draw");  
inline TCustomShowFlag<EShowFlagShippingValue::ForceDisabled> TargetShowFlag(TargetShowFlagName, true, SFG_Normal);


	
	/*----------------------------------------------------------------------------
		Proxy Element
	----------------------------------------------------------------------------*/
FFUDrawProxyElement::FFUDrawProxyElement(FColor InColor, float InTime, uint8 InDepthPriority, uint32 InId):
	Color(InColor),
	DepthPriority(InDepthPriority),
	Id(InId)
{
	if (InTime > 0)
	{
		RemaningTime = InTime;
	}
}

bool FFUDrawProxyElement::Tick(float DeltaTime)
{
	if (float* Value = RemaningTime.GetPtrOrNull())
	{
		*Value -= DeltaTime;
		return (*Value) > 0;
	}
	return true;
}


FFUDrawProxyText::FFUDrawProxyText(const FVector& InLocation, const FString& InText, FColor InColor, float InTime, float InTextSize,
	uint8 InDepthPriority, uint32 InId):
	FFUDrawProxyElement(InColor, InTime, InDepthPriority, InId), 
	Location(InLocation), 
	Text(InText),
	TextSize(InTextSize)
{}


FFUDrawProxySolidLine::FFUDrawProxySolidLine(const FVector& InStartLocation, const FVector& InEndLocation,
	FColor InColor, float InTime, float InThickness, uint8 InDepthPriority, uint32 InId):
	FFUDrawProxyElement(InColor, InTime, InDepthPriority, InId),
	StartLocation(InStartLocation),
	EndLocation(InEndLocation),
	Thickness(InThickness)
{}


FFUDrawProxySphere::FFUDrawProxySphere(const FVector& InLocation, float InRadius, FColor InColor, float InTime,
	float InThickness, FDebugRenderSceneProxy::EDrawType InDrawTypeOverride, uint8 InDepthPriority, uint32 InId):
	FFUDrawProxyElement(InColor, InTime, InDepthPriority, InId),
	Location(InLocation),
	Radius(InRadius),
	Thickness(InThickness),
	DrawTypeOverride(InDrawTypeOverride)
{}


FFUDrawProxyArrow::FFUDrawProxyArrow(const FVector& InStartLocation, const FVector& InEndLocation, FColor InColor,
	float InTime, float InThickness, float InArrowSize, uint8 InDepthPriority, uint32 InId) :
	FFUDrawProxyElement(InColor, InTime, InDepthPriority, InId),
	StartLocation(InStartLocation),
	EndLocation(InEndLocation),
	Thickness(InThickness), 
	ArrowSize(InArrowSize)
{}

	
	/*----------------------------------------------------------------------------
		DrawProxySceneProxy
	----------------------------------------------------------------------------*/
FFUDrawProxySceneProxy::FFUDrawProxySceneProxy(const UFUDrawComponent* InComponent): 
	FDebugRenderSceneProxy(InComponent),
	AdvancedTexts(InComponent->Texts),
	AdvancedSolidLines(InComponent->SolidLines),
	AdvancedSpheres(InComponent->Spheres),
	AdvancedArrows(InComponent->Arrows)
{
	bWillEverBeLit = false;
	ViewFlagName = TargetShowFlagName;
}

FPrimitiveViewRelevance FFUDrawProxySceneProxy::GetViewRelevance(const FSceneView* View) const
{
	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = IsShown(View);
	Result.bDynamicRelevance = true;
	return Result;
}

uint32 FFUDrawProxySceneProxy::GetMemoryFootprint() const
{
	return(sizeof(*this) + GetAllocatedSize());
}

uint32 FFUDrawProxySceneProxy::GetAllocatedSize() const
{
	return FDebugRenderSceneProxy::GetAllocatedSize() + 
		AdvancedSolidLines.GetAllocatedSize() +
		AdvancedSpheres.GetAllocatedSize() +
		AdvancedArrows.GetAllocatedSize();
}

void FFUDrawProxySceneProxy::GetDynamicMeshElementsForView(const FSceneView* View, const int32 ViewIndex,
	const FSceneViewFamily& ViewFamily, const uint32 VisibilityMap, FMeshElementCollector& Collector,
	FMaterialCache& DefaultMaterialCache, FMaterialCache& SolidMeshMaterialCache) const
{
	// copy of FDebugRenderSceneProxy::GetDynamicMeshElementsForView
	
	// TODO: make material inputs to replace DefaultMaterialCache / SolidMeshMaterialCache
	
	TRACE_CPUPROFILER_EVENT_SCOPE(FDebugRenderSceneProxy::GetDynamicMeshElementsForView);
	
	FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
	
	constexpr bool bAllowFrustumCulling = true;
	const FConvexVolume AdjustedFrustum = AdjustViewFrustumForFarClipping(View, FarClippingDistance);;
	
	// Draw ADVANCED Lines
	const int32 LinesNum = AdvancedSolidLines.Num();
	PDI->AddReserveLines(SDPG_World, LinesNum, false, false);
	for (const FFUDrawProxySolidLine& Line : AdvancedSolidLines)
	{
		if (!bAllowFrustumCulling || SegmentInFrustum(Line.StartLocation, Line.EndLocation, AdjustedFrustum))
		{
			PDI->DrawLine(
				Line.StartLocation, Line.EndLocation, 
				Line.Color, Line.GetDepthPriorityGroup(), Line.Thickness, 
				0, Line.Thickness > 0
			);
		}
	}
	
	// Draw Dashed Lines
	for (const FDashedLine& Dash : DashedLines)
	{
		if (!bAllowFrustumCulling || SegmentInFrustum(Dash.Start, Dash.End, AdjustedFrustum))
		{
			Dash.Draw(PDI);
		}
	}
	
	// Draw Circles
	for (const FCircle& Circle : Circles)
	{
		// The idea is to have cheap test, not a precise one : a bounding sphere will do : 
		if (!bAllowFrustumCulling || SphereInFrustum(Circle.Center, Circle.Radius, AdjustedFrustum))
		{
			//Circle.Draw(PDI, (Circle.DrawTypeOverride != EDrawType::Invalid) ? Circle.DrawTypeOverride : DrawType, DrawAlpha, DefaultMaterialCache, ViewIndex, Collector);
		}
	}
	
	// Draw Arrows
	const uint32 ArrowsNum = AdvancedArrows.Num();
	PDI->AddReserveLines(SDPG_World, 5 * ArrowsNum, false, false);
	for (const FFUDrawProxyArrow& ArrowLine : AdvancedArrows)
	{
		if (!bAllowFrustumCulling || SegmentInFrustum(ArrowLine.StartLocation, ArrowLine.EndLocation, AdjustedFrustum))
		{
			FVector Dir = ArrowLine.EndLocation - ArrowLine.StartLocation;
			const float DirMag = Dir.Size();
			Dir /= DirMag;
			FVector YAxis;
			FVector ZAxis;
			Dir.FindBestAxisVectors(YAxis, ZAxis);
			FMatrix ArrowTM(Dir, YAxis, ZAxis, ArrowLine.StartLocation);
			DrawDirectionalArrow(PDI, ArrowTM, ArrowLine.Color, DirMag, ArrowLine.ArrowSize, ArrowLine.GetDepthPriorityGroup());
		}
	}
	
	// Draw Stars
	for (const FWireStar& Star : Stars)
	{
		if (!bAllowFrustumCulling || SphereInFrustum(Star.Position, Star.Size, AdjustedFrustum))
		{
			Star.Draw(PDI);
		}
	}

	// Draw Cylinders
	for (const FWireCylinder& Cylinder : Cylinders)
	{
		// The idea is to have cheap test, not a precise one : a bounding sphere will do : 
		double BoundingSphereRadius = FMath::Sqrt(FMath::Square(Cylinder.HalfHeight) + 2.0 * FMath::Square(Cylinder.Radius));
		if (!bAllowFrustumCulling || SphereInFrustum(Cylinder.Base, BoundingSphereRadius, AdjustedFrustum))
		{
			//Cylinder.Draw(PDI, (Cylinder.DrawTypeOverride != EDrawType::Invalid) ? Cylinder.DrawTypeOverride : DrawType, DrawAlpha, DefaultMaterialCache, ViewIndex, Collector);
		}
	}

	// Draw Boxes
	for (const FDebugBox& Box : Boxes)
	{
		FVector Center, Extents;
		FBox WorldBox = Box.Box.TransformBy(Box.Transform);
		WorldBox.GetCenterAndExtents(Center, Extents);
		if (!bAllowFrustumCulling || BoxInFrustum(Center, Extents, AdjustedFrustum))
		{
			//Box.Draw(PDI, (Box.DrawTypeOverride != EDrawType::Invalid) ? Box.DrawTypeOverride : DrawType, DrawAlpha, DefaultMaterialCache, ViewIndex, Collector);
		}
	}

	// Draw Cones
	for (const FCone& Cone : Cones)
	{
		// The idea is to have a cheap test, not a precise one : a bounding sphere will do : 
		double Angle = FMath::Max(Cone.Angle1, Cone.Angle2) * UE_PI / 180.0;
		double CosAngle = FMath::Cos(Angle);
		FVector SegmentStart = Cone.ConeToWorld.TransformPosition(FVector::ZeroVector);
		FVector SegmentEnd = Cone.ConeToWorld.TransformPosition(FVector::XAxisVector);
		FVector Forward = (SegmentEnd - SegmentStart);
		double ConeLength = Forward.Size();

		FVector SphereCenter;
		double Radius;
		if (Angle > UE_PI / 4.0)
		{
			SphereCenter = SegmentStart + CosAngle * Forward;
			Radius = FMath::Sin(Angle) * ConeLength;
		}
		else
		{
			SphereCenter = SegmentStart + Forward / (2.0 * CosAngle);
			Radius = ConeLength / (2.0 * CosAngle);
		}

		if (!bAllowFrustumCulling || SphereInFrustum(SphereCenter, Radius, AdjustedFrustum))
		{
			TArray<FVector> Verts;
			//Cone.Draw(PDI, (Cone.DrawTypeOverride != EDrawType::Invalid) ? Cone.DrawTypeOverride : DrawType, DrawAlpha, DefaultMaterialCache, ViewIndex, Collector, &Verts);
		}
	}

	// Draw spheres
	for (const FFUDrawProxySphere& Sphere : AdvancedSpheres)
	{
		if (!bAllowFrustumCulling || SphereInFrustum(Sphere.Location, Sphere.Radius, AdjustedFrustum))
		{
			if (Sphere.DrawTypeOverride == SolidAndWireMeshes || Sphere.DrawTypeOverride == WireMesh)
			{
				DrawWireSphere(PDI, Sphere.Location, Sphere.Color, Sphere.Radius, 20, Sphere.GetDepthPriorityGroup(), Sphere.Thickness, 0, true);
			}
			if (Sphere.DrawTypeOverride == SolidAndWireMeshes || Sphere.DrawTypeOverride == SolidMesh)
			{
				//GetSphereMesh(Sphere.Location, FVector(Sphere.Radius), 20, 7, DefaultMaterialCache[Sphere.Color.WithAlpha(DrawAlpha * Sphere.Color.A)], Sphere.GetDepthPriorityGroup(), false, ViewIndex, Collector);
			}
		}
	}

	// Draw Capsules
	for (const FCapsule& Capsule : Capsules)
	{
		// The idea is to have cheap test, not a precise one : a bounding sphere will do : 
		const FVector Origin = Capsule.Base;
		const double HalfAxis = FMath::Max<double>(Capsule.HalfHeight - Capsule.Radius, 1.0);
		const FVector BottomEnd = Capsule.Base;
		const double TotalHalfLength = (HalfAxis + Capsule.Radius);
		const FVector TopEnd = Capsule.Base + 2.0 * TotalHalfLength * Capsule.Z;
		const FVector SphereCenter = (TopEnd + BottomEnd) / 2.0;

		if (!bAllowFrustumCulling || SphereInFrustum(SphereCenter, TotalHalfLength, AdjustedFrustum))
		{
			Capsule.Draw(PDI, (Capsule.DrawTypeOverride != EDrawType::Invalid) ? Capsule.DrawTypeOverride : DrawType, DrawAlpha, DefaultMaterialCache, ViewIndex, Collector);
		}
	}

	// Draw Meshes
	/*for (const FMesh& Mesh : Meshes)
	{
		if (!bAllowFrustumCulling || Mesh.Box.IsValid)
		{
			FVector Center, Extents;
			Mesh.Box.GetCenterAndExtents(Center, Extents);
			if (!BoxInFrustum(Center, Extents, AdjustedFrustum))
			{
				continue;
			}
		}
		FDynamicMeshBuilder MeshBuilder(View->GetFeatureLevel());
		MeshBuilder.AddVertices(Mesh.Vertices);
		MeshBuilder.AddTriangles(Mesh.Indices);

		FMaterialCache& MeshMaterialCache = Mesh.Color.A == 255 ? SolidMeshMaterialCache : DefaultMaterialCache;
		MeshBuilder.GetMesh(FMatrix::Identity, MeshMaterialCache[Mesh.Color], SDPG_World, false, false, ViewIndex, Collector);
	}*/

	// Draw Coordinate Systems
	for (const FCoordinateSystem CoordinateSystem : CoordinateSystems)
	{
		if (!bAllowFrustumCulling || SphereInFrustum(CoordinateSystem.AxisLoc, CoordinateSystem.Scale, AdjustedFrustum))
		{
			CoordinateSystem.Draw(PDI);
		}
	}
}

	
	/*----------------------------------------------------------------------------
		Proxy Delegate helper
	----------------------------------------------------------------------------*/
void FFUDrawProxyDelegateHelper::SetupFromProxy(const FFUDrawProxySceneProxy* InSceneProxy)
{
	AdvancedTexts = InSceneProxy->AdvancedTexts;
}

void FFUDrawProxyDelegateHelper::DrawDebugLabels(UCanvas* Canvas, APlayerController* PlayerController)
{
	//FDebugDrawDelegateHelper::DrawDebugLabels(Canvas, PlayerController);
	
	const FColor OldDrawColor = Canvas->DrawColor;
	const FFontRenderInfo FontRenderInfo = Canvas->CreateFontRenderInfo(true, false);
	const FFontRenderInfo FontRenderInfoWithShadow = Canvas->CreateFontRenderInfo(true, true);
	
	UFont* RenderFont = GEngine->GetSmallFont();
	
	const FSceneView* View = Canvas->SceneView;
	const bool bAllowFrustumCulling = true;
	const FConvexVolume AdjustedFrustum = FDebugRenderSceneProxy::AdjustViewFrustumForFarClipping(View, GetFarClippingDistance()); 
	for (auto It = AdvancedTexts.CreateConstIterator(); It; ++It)
	{
		const FFUDrawProxyText& Text = *It;
		if (!bAllowFrustumCulling || FDebugRenderSceneProxy::PointInFrustum(Text.Location, AdjustedFrustum))
		{
			const FVector3f ScreenLoc = UE::DebugDrawHelper::GetScaleAdjustedScreenLocation(Canvas, Text.Location);
			const FFontRenderInfo& FontInfo = GetTextWithoutShadowDistance() >= 0 
				? (FDebugRenderSceneProxy::PointInRange(Text.Location, View, GetTextWithoutShadowDistance()) 
						? FontRenderInfoWithShadow : FontRenderInfo) : FontRenderInfo;
			Canvas->SetDrawColor(Text.Color);
			Canvas->DrawText(RenderFont, Text.Text, ScreenLoc.X, ScreenLoc.Y, Text.TextSize, Text.TextSize, FontInfo);
		}
	}
	
	Canvas->SetDrawColor(OldDrawColor);
}

	
	/*----------------------------------------------------------------------------
		DrawComponent
	----------------------------------------------------------------------------*/
UFUDrawComponent::UFUDrawComponent()
{
	bAutoActivate = true;
	bTickInEditor = true;
	PrimaryComponentTick.bCanEverTick = true;
	
	UPrimitiveComponent::SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SetGenerateOverlapEvents(false);
	
	bUseEditorCompositing = true;

	// Ignore streaming updates since GetUsedMaterials() is not implemented.
	bIgnoreStreamingManagerUpdate = true;
	
	Texts.Reserve(50);
	SolidLines.Reserve(50);
	Spheres.Reserve(50);
	Arrows.Reserve(50);
}

void UFUDrawComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	bool bElementChanged = false;
	
	
	for (int32 i = Texts.Num() - 1; i >= 0; --i)
	{
		if (!Texts[i].Tick(DeltaTime))
		{
			Texts.RemoveAtSwap(i);
			bElementChanged = true;
		}
	}
	
	for (int32 i = SolidLines.Num() - 1; i >= 0; --i)
	{
		if (!SolidLines[i].Tick(DeltaTime))
		{
			SolidLines.RemoveAtSwap(i);
			bElementChanged = true;
		}
	}
	
	for (int32 i = Spheres.Num() - 1; i >= 0; --i)
	{
		if (!Spheres[i].Tick(DeltaTime))
		{
			Spheres.RemoveAtSwap(i);
			bElementChanged = true;
		}
	}
	
	for (int32 i = Arrows.Num() - 1; i >= 0; --i)
	{
		if (!Arrows[i].Tick(DeltaTime))
		{
			Arrows.RemoveAtSwap(i);
			bElementChanged = true;
		}
	}
	
	
	if (bElementChanged)
	{
		MarkRenderStateDirty();
	}
}

FBoxSphereBounds UFUDrawComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBox BBox(ForceInit);
	
	for (const auto& Text : Texts)
	{
		BBox += Text.Location;
	}
	
	for (const auto& SolidLine : SolidLines)
	{
		BBox += SolidLine.StartLocation;
		BBox += SolidLine.EndLocation;
	}
	
	for (const auto& Sphere : Spheres)
	{
		BBox += Sphere.Location;
	}
	
	for (const auto& Arrow : Arrows)
	{
		BBox += Arrow.StartLocation;
		BBox += Arrow.EndLocation;
	}
	
	return FBoxSphereBounds(BBox);
}

FDebugRenderSceneProxy* UFUDrawComponent::CreateDebugSceneProxy()
{
	auto* Proxy = new FFUDrawProxySceneProxy(this);
	DrawDelegateHelper.SetupFromProxy(Proxy);
	return Proxy;
}

void UFUDrawComponent::ClearForId(uint32 Id)
{
	bool bElementChanged = false;
	
	
	for (int32 i = Texts.Num() - 1; i >= 0; --i)
	{
		if (Texts[i].Id == Id)
		{
			Texts.RemoveAtSwap(i);
			bElementChanged = true;
		}
	}
	
	for (int32 i = SolidLines.Num() - 1; i >= 0; --i)
	{
		if (SolidLines[i].Id == Id)
		{
			SolidLines.RemoveAtSwap(i);
			bElementChanged = true;
		}
	}
	
	for (int32 i = Spheres.Num() - 1; i >= 0; --i)
	{
		if (Spheres[i].Id == Id)
		{
			Spheres.RemoveAtSwap(i);
			bElementChanged = true;
		}
	}
	
	for (int32 i = Arrows.Num() - 1; i >= 0; --i)
	{
		if (Arrows[i].Id == Id)
		{
			Arrows.RemoveAtSwap(i);
			bElementChanged = true;
		}
	}
	
	
	if (bElementChanged)
	{
		MarkRenderStateDirty();
	}
}

void UFUDrawComponent::ClearAll()
{
	Texts.Empty();
	SolidLines.Empty();
	Spheres.Empty();
	Arrows.Empty();
	
	MarkRenderStateDirty();
}

void UFUDrawComponent::DrawDebugText(const FVector& Location, const FString& Text, FColor Color, float Time, float InTextSize, uint32 Id)
{
	Texts.Emplace(Location, Text, Color, Time, InTextSize, Id);
	
	MarkRenderStateDirty();
}

void UFUDrawComponent::DrawDebugSolidLine(const FVector& StartLocation, const FVector& EndLocation, FColor Color,
	float Time, float Thickness, uint32 Id)
{
	SolidLines.Emplace(StartLocation, EndLocation, Color, Time, Thickness, Id);
	
	MarkRenderStateDirty();
}

void UFUDrawComponent::DrawDebugSphere(const FVector& Location, float Radius, FColor Color, float Time, float Thickness,
	FDebugRenderSceneProxy::EDrawType InDrawTypeOverride, uint32 Id)
{
	Spheres.Emplace(Location, Radius, Color, Time, Thickness, InDrawTypeOverride, Id);
	
	MarkRenderStateDirty();
}

void UFUDrawComponent::DrawDebugDirectionalArrow(const FVector& StartLocation, const FVector& EndLocation, FColor Color, 
	float Time, float ArrowSize, float Thickness, uint8 Id)
{
	Arrows.Emplace(StartLocation, EndLocation, Color, Time, Thickness, ArrowSize, Id);
	
	MarkRenderStateDirty();
}

	
	/*----------------------------------------------------------------------------
		DrawProxyActor
	----------------------------------------------------------------------------*/
AFUDrawProxyActor::AFUDrawProxyActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	auto* Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	
	DrawComponent = CreateDefaultSubobject<UFUDrawComponent>("DrawComponent");
	DrawComponent->SetupAttachment(RootComponent.Get());
}
