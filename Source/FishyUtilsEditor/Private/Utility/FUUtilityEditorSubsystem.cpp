// Copyright Brutal Sun Dev Team. All Rights Reserved.


#include "Utility/FUUtilityEditorSubsystem.h"
#include "Selection.h"
#include "Core/FUEditorSettings.h"
#include "Utility/FUEditorUtilities.h"
#include "GameFramework/ActorPrimitiveColorHandler.h"


#if ENABLE_ACTOR_PRIMITIVE_COLOR_HANDLER
static FName NAME_MobilityColor(TEXT("MobilityColor"));
static FName NAME_FarShadowColor(TEXT("FarShadowColor"));
static FName NAME_NegativeScaleColor(TEXT("NegativeScaleColor"));
static FName NAME_TwoSidedColor(TEXT("TwoSidedColor"));
static FName NAME_DetailModeColor(TEXT("DetailModeColor"));
static FName NAME_CollisionEnabled(TEXT("CollisionEnabled"));
static FName NAME_GenerateOverlapEnabled(TEXT("GenerateOverlapEnabled"));
#endif


UFUUtilityEditorSubsystem::FFUMirrorEntry::FFUMirrorEntry(AActor* InActor):
	Actor(InActor), OriginalTransform(InActor->GetTransform())
{}


void UFUUtilityEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	EditorSettings = GetDefault<UFUEditorSettings>();
	
	if (EditorSettings->bEnableMirrorPreview)
	{
		TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &ThisClass::Tick), 0);
	}
	
#if ENABLE_ACTOR_PRIMITIVE_COLOR_HANDLER
	FActorPrimitiveColorHandler::Get().RegisterPrimitiveColorHandler(
		NAME_MobilityColor,
		INVTEXT("FU: Mobility"),
		[this] (const UPrimitiveComponent* InPrimitiveComponent) -> FLinearColor 
		{
			FLinearColor OutColor = FLinearColor::Gray;
			
			switch (InPrimitiveComponent->GetMobility())
			{
				case EComponentMobility::Stationary:
					{
						OutColor = FLinearColor::Yellow;
					   break;
					}
				case EComponentMobility::Movable:
					{
						OutColor = FLinearColor::Red;
					   break;
					}
				default:
					break;
			}
			return OutColor + GetColorVariation();
		},
		[]() {},
		INVTEXT("Colorize primitives to show their mobility. Gray = Static, Yellow = Stationary, Red = Movable.")
	);
	
	FActorPrimitiveColorHandler::Get().RegisterPrimitiveColorHandler(
		NAME_FarShadowColor,
		INVTEXT("FU: Far Shadows"),
		[this] (const UPrimitiveComponent* InPrimitiveComponent) -> FLinearColor 
		{
			FLinearColor OutColor = InPrimitiveComponent->bCastFarShadow ? FLinearColor::Green : FLinearColor::Gray;
			
			return OutColor + GetColorVariation();
		},
		[]() {},
		INVTEXT("Highlights components casting far shadows")
	);
	
	FActorPrimitiveColorHandler::Get().RegisterPrimitiveColorHandler(
		NAME_NegativeScaleColor,
		INVTEXT("FU: Negative Scale"),
		[this] (const UPrimitiveComponent* InPrimitiveComponent) -> FLinearColor 
		{
			const FVector Scale = InPrimitiveComponent->GetComponentScale();
			if (Scale.X < 0 || Scale.Y < 0 || Scale.Z < 0)
			{
				return FLinearColor::Red + GetColorVariation();
			}
			return FLinearColor::Gray + GetColorVariation();
		},
		[]() {},
		INVTEXT("Highlights components with negative scales")
	);
	
	FActorPrimitiveColorHandler::Get().RegisterPrimitiveColorHandler(
		NAME_TwoSidedColor,
		INVTEXT("FU: Two Sided"),
		[this] (const UPrimitiveComponent* InPrimitiveComponent) -> FLinearColor 
		{
			FLinearColor OutColor = FLinearColor::Gray;
			TArray<UMaterialInterface*> UsedMaterials;
			InPrimitiveComponent->GetUsedMaterials(UsedMaterials);
			for (const UMaterialInterface* Material : UsedMaterials)
			{
				if (!IsValid(Material))
				{
					continue;
				}
				if (Material->IsTwoSided())
				{
					OutColor = FLinearColor::Red;
					break;
				}
			}
			return OutColor + GetColorVariation();
		},
		[]() {},
		INVTEXT("Highlights components that use two-sided materials")
	);

	FActorPrimitiveColorHandler::Get().RegisterPrimitiveColorHandler(
		NAME_DetailModeColor,
		INVTEXT("FU: Detail Mode"),
		[this] (const UPrimitiveComponent* InPrimitiveComponent) -> FLinearColor 
		{
			FLinearColor OutColor = FLinearColor::Gray;
			switch (InPrimitiveComponent->DetailMode)
			{
			case EDetailMode::DM_Epic:
				OutColor = FLinearColor::Red;
				break;
			case EDetailMode::DM_High:
				OutColor = FLinearColor::Yellow;
				break;
			case EDetailMode::DM_Medium:
				OutColor = FLinearColor::Green;
			default: ;
			}
			return OutColor + GetColorVariation();
		},
		[]() {},
		INVTEXT(
			"Visualizes the detail modes assigned to primitives.\nGray primitives are always visible. \nGreen "
			"primitives are visible at medium settings and higher \nYellow primitives are visible at high settings and "
			"higher \nRed primitives are visible at epic settings and higher")
	);
	
	FActorPrimitiveColorHandler::Get().RegisterPrimitiveColorHandler(
		NAME_CollisionEnabled,
		INVTEXT("FU: Collision Enabled"),
		[this] (const UPrimitiveComponent* InPrimitiveComponent) -> FLinearColor 
		{
			FLinearColor OutColor = InPrimitiveComponent->IsCollisionEnabled() ? FLinearColor::Red : FLinearColor::Gray;
			return OutColor + GetColorVariation();
		},
	[]() {},
	INVTEXT("Colorize primitives depening on Collision Enabled. Gray = Disabled, Red = Enabled.")
	);
	
	FActorPrimitiveColorHandler::Get().RegisterPrimitiveColorHandler(
		NAME_GenerateOverlapEnabled,
		INVTEXT("FU: Generate Overlap Enabled"),
		[this] (const UPrimitiveComponent* InPrimitiveComponent) -> FLinearColor 
		{
			if (FU::EditorUtilities::ShouldGenerateOverlapEventsComponentBeIgnored(InPrimitiveComponent))
			{
				return FLinearColor::Gray + GetColorVariation();
			}

			FLinearColor OutColor;
			switch (FU::EditorUtilities::GetGenerateOverlapEventsInfoForComponent(InPrimitiveComponent))
			{
				case FU::EditorUtilities::GenerateOverlapDisabled:
					{
						OutColor = FLinearColor::Gray;
						break;
					}
				case FU::EditorUtilities::NoCollision:
					{
						OutColor = FLinearColor::Blue;
						break;
					}
				case FU::EditorUtilities::OverlapCollisionResponseUsedOnly:
					{
						OutColor = FLinearColor::Yellow;
						break;
					}
				case FU::EditorUtilities::OverlapCollisionResponseUsedAndDelegatesBound:
					{
						OutColor = FLinearColor::Yellow;
						break;
					}
				case FU::EditorUtilities::DelegatesBoundOnly:
					{
						OutColor = FLinearColor::Red;
						break;
					}
				case FU::EditorUtilities::GenerateOverlapEnabled:
					{
						OutColor = FLinearColor::Red;
						break;
					}
				default: ;
			}
			
			return OutColor + GetColorVariation();
		},
	[]() {},
	INVTEXT("Colorize primitives depening on their \"Generate Overlap Events\" state.\n"
		 "Gray: GenerateOverlapEvents is disabled or it isn't considered.\n"
		 "Blue: GenerateOverlapEvents is enabled but the profile is set to NoCollision\n"
		 "Yellow: GenerateOverlapEvents is enabled and Overlap is used as a Collision Response\n"
		 "Red: GenerateOverlapEnabled is enabled")
	);
#endif
}

void UFUUtilityEditorSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	if (TickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
	}
	
#if ENABLE_ACTOR_PRIMITIVE_COLOR_HANDLER
	FActorPrimitiveColorHandler::Get().UnregisterPrimitiveColorHandler(NAME_MobilityColor);
	FActorPrimitiveColorHandler::Get().UnregisterPrimitiveColorHandler(NAME_FarShadowColor);
	FActorPrimitiveColorHandler::Get().UnregisterPrimitiveColorHandler(NAME_NegativeScaleColor);
	FActorPrimitiveColorHandler::Get().UnregisterPrimitiveColorHandler(NAME_TwoSidedColor);
	FActorPrimitiveColorHandler::Get().UnregisterPrimitiveColorHandler(NAME_DetailModeColor);
	FActorPrimitiveColorHandler::Get().UnregisterPrimitiveColorHandler(NAME_CollisionEnabled);
	FActorPrimitiveColorHandler::Get().UnregisterPrimitiveColorHandler(NAME_GenerateOverlapEnabled);
#endif
}

bool UFUUtilityEditorSubsystem::Exec_Editor(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (FParse::Command(&Cmd, TEXT("ELEMENT MIRROR")))
	{
		StopCurrentMirrorType(true);
	}
	
	return false;
}

bool UFUUtilityEditorSubsystem::Tick(float DeltaTime)
{
	if (FSlateApplication::IsInitialized() && !GEditor->IsPlaySessionInProgress() && FSlateThrottleManager::Get().IsAllowingExpensiveTasks())
	{
		const UE::Slate::FDeprecateVector2DResult CursorPosition = FSlateApplication::Get().GetCursorPos();
		const FWidgetPath WidgetPath = FSlateApplication::Get().LocateWindowUnderMouse(
			CursorPosition, 
			FSlateApplication::Get().GetInteractiveTopLevelWindows()
		);
		
		if (WidgetPath.IsValid())
		{
			if (LastTickHoveredWidget != WidgetPath.GetLastWidget())
			{
				LastTickHoveredWidget = WidgetPath.GetLastWidget();
				
				int32 MenuEntryBlockIndex = INDEX_NONE;
				for (int32 i = WidgetPath.Widgets.Num() - 1; i >= 0; --i)
				{
					const FArrangedWidget& ArrangedWidget = WidgetPath.Widgets[i];
					if (ArrangedWidget.Widget->GetType() == "SMenuEntryBlock")
					{
						MenuEntryBlockIndex = i;
						break;
					}
				}
				
				if (MenuEntryBlockIndex != INDEX_NONE)
				{
					bool bFound = false;
					for (int32 i = WidgetPath.Widgets.Num() - 1; i >= MenuEntryBlockIndex; --i)
					{
						const FArrangedWidget& ArrangedWidget = WidgetPath.Widgets[i];
						if (ArrangedWidget.Widget->GetType() == "STextBlock")
						{
							using namespace FU::EditorUtilities;
							
							EFUMirrorType Type = EFUMirrorType::None;
							
							TSharedRef<STextBlock> TextBlock = StaticCastSharedRef<STextBlock>(ArrangedWidget.Widget);
							const FString Text = TextBlock->GetText().ToString();
							if (Text == "Mirror X Axis")
							{
								Type = EFUMirrorType::X;
							}
							else if (Text == "Mirror Y Axis")
							{
								Type = EFUMirrorType::Y;
							}
							else if (Text == "Mirror Z Axis")
							{
								Type = EFUMirrorType::Z;
							}
							
							if (Type != EFUMirrorType::None)
							{
								bFound = true;
								
								if (Type != HandledMirrorType)
								{
									StartMirrorType(Type);
								}
							}
							
							break;
						}
					}
					
					if (!bFound)
					{
						StopCurrentMirrorType(true);
					}
				}
				else
				{
					StopCurrentMirrorType(true);
				}
			}
		}
		else
		{
			StopCurrentMirrorType(true);
			
			LastTickHoveredWidget.Reset();
		}
	}
	
	return true;
}

	
	/*----------------------------------------------------------------------------
		Mirror
	----------------------------------------------------------------------------*/
void UFUUtilityEditorSubsystem::StartMirrorType(EFUMirrorType Type)
{
	if (HandledMirrorType == Type) { return; }
		
	StopCurrentMirrorType(false);
	
	HandledMirrorType = Type;
	
	TArray<AActor*> SelectedActors;
	GEditor->GetSelectedActors()->GetSelectedObjects(SelectedActors);
	
	HandledMirrorEntries.Reserve(SelectedActors.Num());
	for (auto* SelectedActor : SelectedActors)
	{
		HandledMirrorEntries.Emplace(SelectedActor);
		
		const FVector MirrorScale = FVector(Type == EFUMirrorType::X ? -1 : 1, Type == EFUMirrorType::Y ? -1 : 1, Type == EFUMirrorType::Z ? -1 : 1);
		SelectedActor->SetActorScale3D(SelectedActor->GetActorScale3D() * MirrorScale);
	}
}

void UFUUtilityEditorSubsystem::StopCurrentMirrorType(bool bRestore)
{
	using namespace FU::EditorUtilities;
	
	if (!LastTickHoveredWidget.IsValid()) { return; }
	if (HandledMirrorType == EFUMirrorType::None) { return; }
	
	HandledMirrorType = EFUMirrorType::None;
	
	if (bRestore)
	{
		for (auto& Entry : HandledMirrorEntries)
		{
			if (Entry.Actor.IsValid())
			{
				Entry.Actor->SetActorTransform(Entry.OriginalTransform);
			}
		}
	}
	HandledMirrorEntries.Empty();
}

FLinearColor UFUUtilityEditorSubsystem::GetColorVariation() const
{
	const float Variation = FMath::FRand() * 0.2 - 0.1;
	return FLinearColor(Variation, Variation, Variation);
}
