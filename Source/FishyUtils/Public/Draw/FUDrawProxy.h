// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "DebugRenderSceneProxy.h"
#include "Debug/DebugDrawComponent.h"
#include "GameFramework/Actor.h"
#include "FUDrawProxy.generated.h"


struct FFUDrawProxyElement
{
	FFUDrawProxyElement(FColor InColor, float InTime, uint8 InDepthPriority = 0, uint32 InId = 0);
	
	FColor Color;
	/** If not set this is infinite */
	float RemainingTime;
	uint8 DepthPriority;
	uint32 Id;
	
	uint8 GetDepthPriorityGroup() const
	{
		return DepthPriority == 0 ? SDPG_World : SDPG_Foreground;
	}
	
	/** @returns true if still alive */
	bool Tick(float DeltaTime);
};


struct FFUDrawProxyText : public FFUDrawProxyElement
{
	FFUDrawProxyText(const FVector& InLocation, const FString& InText, FColor InColor, float InTime, float InTextSize = 1,
		uint8 InDepthPriority = 0, uint32 InId = 0);
	
	FVector Location;
	FString Text;
	float TextSize;
};


struct FFUDrawProxySolidLine : public FFUDrawProxyElement
{
	FFUDrawProxySolidLine(const FVector& InStartLocation, const FVector& InEndLocation, FColor InColor, float InTime, 
		float InThickness = 2, uint8 InDepthPriority = 0, uint32 InId = 0);
	
	FVector StartLocation;
	FVector EndLocation;
	float Thickness;
};


struct FFUDrawProxySphere : public FFUDrawProxyElement
{
	FFUDrawProxySphere(const FVector& InLocation, float InRadius, FColor InColor, float InTime, float InThickness = 2, 
		FDebugRenderSceneProxy::EDrawType InDrawTypeOverride = FDebugRenderSceneProxy::EDrawType::Invalid, 
		uint8 InDepthPriority = 0, uint32 InId = 0);
	
	FVector Location;
	float Radius;
	float Thickness;
	FDebugRenderSceneProxy::EDrawType DrawTypeOverride;
};


struct FFUDrawProxyArrow : public FFUDrawProxyElement
{
	FFUDrawProxyArrow(const FVector& InStartLocation, const FVector& InEndLocation, FColor InColor, float InTime, 
		float InThickness = 2, float InArrowSize = 2, uint8 InDepthPriority = 0, uint32 InId = 0);
	
	FVector StartLocation;
	FVector EndLocation;
	float Thickness;
	float ArrowSize;
};


class FFUDrawProxySceneProxy : public FDebugRenderSceneProxy
{
	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
public:
	TArray<FFUDrawProxyText> AdvancedTexts;
protected:
	TArray<FFUDrawProxySolidLine> AdvancedSolidLines;
	TArray<FFUDrawProxySphere> AdvancedSpheres;
	TArray<FFUDrawProxyArrow> AdvancedArrows;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	FFUDrawProxySceneProxy(const UFUDrawComponent* InComponent);
	
	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;
	
	virtual uint32 GetMemoryFootprint(void) const override;
	
	uint32 GetAllocatedSize( void ) const;
	
protected:
	virtual void GetDynamicMeshElementsForView(const FSceneView* View, const int32 ViewIndex, const FSceneViewFamily& ViewFamily, const uint32 VisibilityMap, FMeshElementCollector& Collector, FMaterialCache& DefaultMaterialCache, FMaterialCache& SolidMeshMaterialCache) const override;
};


class FFUDrawProxyDelegateHelper : public FDebugDrawDelegateHelper
{
	
public:
	void SetupFromProxy(const FFUDrawProxySceneProxy* InSceneProxy);
	
protected:
	TArray<FFUDrawProxyText> AdvancedTexts;
	
	virtual void DrawDebugLabels(UCanvas* Canvas, APlayerController*) override;
};


UCLASS(Hidden)
class FISHYUTILS_API UFUDrawComponent : public UDebugDrawComponent
{
	GENERATED_BODY()
	
	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
public:
	TArray<FFUDrawProxyText> Texts;
	TArray<FFUDrawProxySolidLine> SolidLines;
	TArray<FFUDrawProxySphere> Spheres;
	TArray<FFUDrawProxyArrow> Arrows;
	
protected:
	FFUDrawProxyDelegateHelper DrawDelegateHelper;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UFUDrawComponent();
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	
protected:
	virtual FDebugRenderSceneProxy* CreateDebugSceneProxy() override;
	
	virtual FDebugDrawDelegateHelper& GetDebugDrawDelegateHelper() override { return DrawDelegateHelper; }
	
	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
public:
	void ClearForId(uint32 Id);
	
	void ClearAll();
	
	void DrawDebugText(const FVector& Location, const FString& Text, FColor Color, float Time, float InTextSize = 1, uint32 Id = 0);
	
	void DrawDebugSolidLine(const FVector& StartLocation, const FVector& EndLocation, FColor Color, float Time, 
		float Thickness = 2, uint32 Id = 0);

	void DrawDebugSphere(const FVector& Location, float Radius, FColor Color, float Time, float Thickness = 2, 
		FDebugRenderSceneProxy::EDrawType InDrawTypeOverride = FDebugRenderSceneProxy::EDrawType::Invalid, uint32 Id = 0);
	
	void DrawDebugDirectionalArrow(const FVector& StartLocation, const FVector& EndLocation, FColor Color, float Time,
		float ArrowSize = 2, float Thickness = 2, uint8 Id = 0);
};


UCLASS(Hidden)
class FISHYUTILS_API AFUDrawProxyActor : public AActor
{
	GENERATED_BODY()

	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
public:
	UPROPERTY(BlueprintReadOnly, Category="FishyUtils")
	TObjectPtr<UFUDrawComponent> DrawComponent;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	AFUDrawProxyActor();
};

