// Copyright Brutal Sun Dev Team. All Rights Reserved.


#include "Utility/FUUtilityEditorSubsystem.h"
#include "Selection.h"
#include "Utility/FUEditorUtilities.h"


UFUUtilityEditorSubsystem::FFUMirrorEntry::FFUMirrorEntry(AActor* InActor):
	Actor(InActor), OriginalTransform(InActor->GetTransform())
{}


void UFUUtilityEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
		
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &ThisClass::Tick), 0);
}

void UFUUtilityEditorSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
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
