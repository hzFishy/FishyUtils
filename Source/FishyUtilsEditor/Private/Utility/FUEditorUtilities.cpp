// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "Utility/FUEditorUtilities.h"
#include "EditorCategoryUtils.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"
#include "LevelEditor.h"
#include "Selection.h"
#include "Framework/Commands/GenericCommands.h"
#include "Misc/UObjectToken.h"

#define LOCTEXT_NAMESPACE "FFishyUtilsModule"


namespace FU_EditorUtilities
{
	
	/*----------------------------------------------------------------------------
		Overlap Detection
	----------------------------------------------------------------------------*/
	static int32 PrintAllGenerateOverlapEventsComponentsFromAssetsCount = 0;
	static TMap<FString, int32> PrintAllGenerateOverlapEventsComponentsFromPathCount;
	static TMap<const UWorld*, int32> PrintAllGenerateOverlapEventsComponentsFromWorldCount;
	
	FString GenerateOverlapEventsResultToString(EFUGenerateOverlapEventsResult Result)
	{
		switch (Result) 
		{
		case GenerateOverlapEnabled:
			return "GenerateOverlapEvents is enabled";
		case NoCollision:
			return "GenerateOverlapEvents is enabled but the profile is set to NoCollision";
		case OverlapCollisionResponseUsedOnly:
			return "GenerateOverlapEvents is enabled and Overlap is used as a Collision Response";
		case GenerateOverlapDisabled:
			return "GenerateOverlapEvents is disabled";
		case OverlapCollisionResponseUsedAndDelegatesBound:
			return "GenerateOverlapEvents is enabled, Overlap is used as a Collision Response and Begin/End overlap delegates are bound";
		case DelegatesBoundOnly:
			return "GenerateOverlapEvents is enabled and Begin/End overlap delegates are bound";
		}
		
		return "N/A";
	}
	
	void PrintAllGenerateOverlapEventsComponentsFromAssets()
	{
		PrintAllGenerateOverlapEventsComponentsFromAssetsCount++;
		
		FMessageLog("FishyUtils").NewPage(FText::FromString(FString::Printf(TEXT("PrintAllGenerateOverlapEventsComponentsFromAssets (%i)"), 
			PrintAllGenerateOverlapEventsComponentsFromAssetsCount)));
		FMessageLog("FishyUtils").Info()
			->AddText(INVTEXT("All info about Primitive Components with bGenerateOverlapEvents enabled for all project assets"));
		
		auto& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		
		// TODO: order by result
		
		TArray<FAssetData> ObjectList;
		AssetRegistryModule.Get().GetAssetsByClass(UBlueprint::StaticClass()->GetClassPathName(), ObjectList, true);
		for (auto ObjIter = ObjectList.CreateConstIterator(); ObjIter; ++ObjIter) 
		{
			const FAssetData& Asset = *ObjIter;
			if (Asset.GetSoftObjectPath().ToString().Contains("/Game"))
			{
				UBlueprint* BP = Cast<UBlueprint>(Asset.GetAsset());
				UClass* AssetClass = BP->GeneratedClass;
				
				if (AssetClass->IsChildOf<AActor>())
				{
					TArray<const UPrimitiveComponent*> Components;
					AActor::GetActorClassDefaultComponents<UPrimitiveComponent>(AssetClass, Components);
					
					if (!Components.IsEmpty())
					{
						for (auto* Component : Components)
						{
							if (ShouldGenerateOverlapEventsComponentBeIgnored(Component))
							{
								continue;
							}
							
							EFUGenerateOverlapEventsResult Result = GetGenerateOverlapEventsInfoForComponent(Component);
							
							if (Result == EFUGenerateOverlapEventsResult::GenerateOverlapDisabled)
							{
								continue;
							}
							
							BuildMessageForGenerateOverlapEventsForComponent(Component, Asset, Result);
						}
					}
				}
			}
		}
	}
	
	void FU_EditorUtilities::PrintAllGenerateOverlapEventsComponentsFromWorld(UWorld* World)
	{
		if (PrintAllGenerateOverlapEventsComponentsFromWorldCount.Contains(World))
		{
			PrintAllGenerateOverlapEventsComponentsFromWorldCount[World] += 1;
		}
		else
		{
			PrintAllGenerateOverlapEventsComponentsFromWorldCount.Add(World, 1);
		}
		
		FMessageLog("FishyUtils").NewPage(FText::FromString(FString::Printf(TEXT("PrintAllGenerateOverlapEventsComponentsFromWorld (%s) (%i)"), 
			*World->GetName(), PrintAllGenerateOverlapEventsComponentsFromWorldCount[World])));
		FMessageLog("FishyUtils").Info()
			->AddText(FText::FromString(FString::Printf(TEXT("All info about Primitive Components with bGenerateOverlapEvents enabled for all actors in world %s"), *World->GetName())));
		
		for (auto It = TActorIterator<AActor>(World, AActor::StaticClass()); It; ++It)
		{
			AActor* Actor = *It;
			TArray<UPrimitiveComponent*> RawComponents;
			Actor->GetComponents<UPrimitiveComponent>(RawComponents);
			
			if (!RawComponents.IsEmpty())
			{
				for (UPrimitiveComponent* Component : RawComponents)
				{
					if (ShouldGenerateOverlapEventsComponentBeIgnored(Component))
					{
						continue;
					}
					
					EFUGenerateOverlapEventsResult Result = GetGenerateOverlapEventsInfoForComponent(Component);
					
					if (Result == EFUGenerateOverlapEventsResult::GenerateOverlapDisabled)
					{
						continue;
					}
					
					auto* BPAsset = Actor->GetClass()->ClassGeneratedBy.Get();
					
					FMessageLog("FishyUtils").Info()
						->AddText(FText::FromString(FString::Printf(TEXT("%s for %s - "), *GenerateOverlapEventsResultToString(Result), *FU::Utils::GetObjectDetailedName(Component))))
						->AddToken(FActorToken::Create(Actor->GetPathName(), Actor->GetActorGuid(), INVTEXT("Focus Actor")))
						->AddToken(FActionToken::Create(INVTEXT("Open Blueprint"), FText::FromString(IsValid(BPAsset) ? "Open Blueprint" : "No Blueprint asset found"), FOnActionTokenExecuted::CreateLambda([BPAsset] ()
						{
							GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(BPAsset);
						}), FCanExecuteActionToken::CreateLambda([BPAsset] ()
						{
							return IsValid(BPAsset);
						})))
						->AddToken(FActionToken::Create(INVTEXT("Disable GenerateOverlapEvents on instance"), INVTEXT("Disable GenerateOverlapEvents"), FOnActionTokenExecuted::CreateLambda([Component] ()
						{
							auto* MutableComponent = const_cast<UPrimitiveComponent*>(Component);
							MutableComponent->SetGenerateOverlapEvents(false);
							MutableComponent->MarkPackageDirty();
						}), true))
					;
				}
			}
		}
	}

	void FU_EditorUtilities::PrintAllGenerateOverlapEventsComponentsFromPath(const FString& Path)
	{
		if (PrintAllGenerateOverlapEventsComponentsFromPathCount.Contains(Path))
		{
			PrintAllGenerateOverlapEventsComponentsFromPathCount[Path] += 1;
		}
		else
		{
			PrintAllGenerateOverlapEventsComponentsFromPathCount.Add(Path, 1);
		}
		
		FMessageLog("FishyUtils").NewPage(FText::FromString(FString::Printf(TEXT("PrintAllGenerateOverlapEventsComponentsFromPath (%s) (%i)"), 
			*Path, PrintAllGenerateOverlapEventsComponentsFromPathCount[Path])));
		FMessageLog("FishyUtils").Info()
			->AddText(FText::FromString(FString::Printf(TEXT("All info about Primitive Components with bGenerateOverlapEvents enabled for all project assets in root folder %s"), *Path)));
		
		auto& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		
		// TODO: order by result
		
		TArray<FAssetData> ObjectList;
		AssetRegistryModule.Get().GetAssetsByPath(FName(Path), ObjectList, true);
		for (auto ObjIter = ObjectList.CreateConstIterator(); ObjIter; ++ObjIter) 
		{
			const FAssetData& Asset = *ObjIter;
			if (UBlueprint* BP = Cast<UBlueprint>(Asset.GetAsset()))
			{
				UClass* AssetClass = BP->GeneratedClass;
			
				if (AssetClass->IsChildOf<AActor>())
				{
					TArray<const UPrimitiveComponent*> Components;
					AActor::GetActorClassDefaultComponents<UPrimitiveComponent>(AssetClass, Components);
				
					if (!Components.IsEmpty())
					{
						for (auto* Component : Components)
						{
							if (ShouldGenerateOverlapEventsComponentBeIgnored(Component))
							{
								continue;
							}
							
							EFUGenerateOverlapEventsResult Result = GetGenerateOverlapEventsInfoForComponent(Component);
							
							if (Result == EFUGenerateOverlapEventsResult::GenerateOverlapDisabled)
							{
								continue;
							}
							
							BuildMessageForGenerateOverlapEventsForComponent(Component, Asset, Result);
						}
					}
				}
			}
		}
	}
	
	EFUGenerateOverlapEventsResult GetGenerateOverlapEventsInfoForComponent(const UPrimitiveComponent* Component)
	{
		if (Component->GetGenerateOverlapEvents())
		{
			if (Component->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
			{
				return EFUGenerateOverlapEventsResult::NoCollision;
			}
			else
			{
				// check if we are using ECR_Response for a channel
				auto& Resps = Component->GetCollisionResponseToChannels();
				bool bAnyOverlapResp = false;
				for (int32 i = 0; i < std::size(Resps.EnumArray); ++i)
				{
					if (Resps.GetResponse(static_cast<ECollisionChannel>(i)) == ECR_Overlap)
					{
						bAnyOverlapResp = true;
						break;
					}
				}
				
				// TODO: fix, doesnt detect bound delegate in BP graphs
				// check if bounds delegates
				const bool bDelegateBound = Component->OnComponentBeginOverlap.IsBound() || Component->OnComponentEndOverlap.IsBound();
				
				if (bAnyOverlapResp && bDelegateBound)
				{
					return EFUGenerateOverlapEventsResult::OverlapCollisionResponseUsedAndDelegatesBound;
				}
				else if (bAnyOverlapResp)
				{
					return EFUGenerateOverlapEventsResult::OverlapCollisionResponseUsedOnly;
				}
				else if (bDelegateBound)
				{
					return EFUGenerateOverlapEventsResult::DelegatesBoundOnly;
				}
			}
		}
		else
		{
			return EFUGenerateOverlapEventsResult::GenerateOverlapDisabled;
		}
		
		return EFUGenerateOverlapEventsResult::GenerateOverlapEnabled;
	}

	bool ShouldGenerateOverlapEventsComponentBeIgnored(const UPrimitiveComponent* Component)
	{
		TArray<FString> HideCategoriesArray;
		FEditorCategoryUtils::GetClassHideCategories(Component->GetClass(), HideCategoriesArray);
							
		// ignore the component if its collision properties are hidden
		if (!HideCategoriesArray.IsEmpty() && (HideCategoriesArray.Contains("Collision") || HideCategoriesArray.Contains("Physics")))
		{
			return true;
		}
		return false;
	}

	void BuildMessageForGenerateOverlapEventsForComponent(const UPrimitiveComponent* Component, const FAssetData& Asset, EFUGenerateOverlapEventsResult Result)
	{
		FMessageLog("FishyUtils").Info()
			->AddText(FText::FromString(FString::Printf(TEXT("%s for %s - "), *GenerateOverlapEventsResultToString(Result), *FU::Utils::GetObjectDetailedName(Component))))
			->AddToken(FUObjectToken::Create(Asset.GetAsset(), INVTEXT("Select in Content Browser")))
			->AddToken(FActionToken::Create(INVTEXT("Open Blueprint"), INVTEXT("Open Blueprint"), FOnActionTokenExecuted::CreateLambda([Asset] ()
			{
				GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Asset.GetAsset());
			})))
			->AddToken(FActionToken::Create(INVTEXT("Disable GenerateOverlapEvents"), INVTEXT("Disable GenerateOverlapEvents"), FOnActionTokenExecuted::CreateLambda([Component] ()
			{
				auto* MutableComponent = const_cast<UPrimitiveComponent*>(Component);
				MutableComponent->SetGenerateOverlapEvents(false);
				MutableComponent->MarkPackageDirty();
			}), true))
		;
	}
	
	
	/*----------------------------------------------------------------------------
		Collision detection
	----------------------------------------------------------------------------*/
	static TMap<const UWorld*, int32> PrintAllCollisionEnabledComponentsFromWorldCount;
	
	void PrintAllCollisionEnabledComponentsFromWorld(UWorld* World, bool bHideActorsWithNoCollision)
	{
		if (PrintAllCollisionEnabledComponentsFromWorldCount.Contains(World))
		{
			PrintAllCollisionEnabledComponentsFromWorldCount[World] += 1;
		}
		else
		{
			PrintAllCollisionEnabledComponentsFromWorldCount.Add(World, 1);
		}
		
		FMessageLog("FishyUtils").NewPage(FText::FromString(FString::Printf(TEXT("PrintAllCollisionEnabledComponentsFromWorld (%s) (%i)"), 
			*World->GetName(), PrintAllCollisionEnabledComponentsFromWorldCount[World])));
		FMessageLog("FishyUtils").Info()
			->AddText(FText::FromString(FString::Printf(TEXT("All info about Primitive Components with Collision Enabled for all actors in world %s"), *World->GetName())));
		
		for (auto It = TActorIterator<AActor>(World, AActor::StaticClass()); It; ++It)
		{
			AActor* Actor = *It;
			TArray<UPrimitiveComponent*> RawComponents;
			Actor->GetComponents<UPrimitiveComponent>(RawComponents);
			
			if (!RawComponents.IsEmpty())
			{
				bool bCollisions = false;
				for (UPrimitiveComponent* Component : RawComponents)
				{
					bool bCollisionEnabled = Component->IsCollisionEnabled();
					
					if (bCollisionEnabled)
					{
						bCollisions = true;
						auto* BPAsset = Actor->GetClass()->ClassGeneratedBy.Get();
						
						FMessageLog("FishyUtils").Info()
							->AddText(FText::FromString(FString::Printf(TEXT("Collision enabled for %s - "), *FU::Utils::GetObjectDetailedName(Component))))
							->AddToken(FActorToken::Create(Actor->GetPathName(), Actor->GetActorGuid(), INVTEXT("Focus Actor")))
							->AddToken(FActionToken::Create(INVTEXT("Open Blueprint"), FText::FromString(IsValid(BPAsset) ? "Open Blueprint" : "No Blueprint asset found"), FOnActionTokenExecuted::CreateLambda([BPAsset] ()
							{
								GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(BPAsset);
							}), FCanExecuteActionToken::CreateLambda([BPAsset] ()
							{
								return IsValid(BPAsset);
							})))
							->AddToken(FActionToken::Create(INVTEXT("Set profile to NoCollision on instance"), INVTEXT("Set profile to NoCollision on instance"), FOnActionTokenExecuted::CreateLambda([Component] ()
							{
								auto* MutableComponent = const_cast<UPrimitiveComponent*>(Component);
								MutableComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
								MutableComponent->MarkPackageDirty();
							}), true))
						;
					}
				}
				
				if (!bCollisions && bHideActorsWithNoCollision)
				{
					Actor->SetIsTemporarilyHiddenInEditor(true);
				}
			}
		}
	}
}


FFUEditor::FFUEditor() {}

FFUEditor::~FFUEditor() {}

void FFUEditor::RegisterMenuExtensions()
{
	// Use the current object as the owner of the menus
	// This allows us to remove all our custom menus when the module is unloaded
	FToolMenuOwnerScoped OwnerScoped(this);
	
	auto* ToolMenus = UToolMenus::Get();
	auto& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	const TSharedRef<FUICommandList> Commands = LevelEditor.GetGlobalLevelEditorActions();
	
	
	// Select menu
	{
		auto* SelectionMenu = ToolMenus->ExtendMenu("LevelEditor.MainMenu.Select");
		
		FToolMenuSection& FishySelectionSection = SelectionMenu->AddSection(
			"FishyUtilities",
			INVTEXT("Fishy Utilities")
		);
		
		// SelectSameFolderLevel
		{
			FishySelectionSection.AddMenuEntry(FFUEditorCommands::Get().SelectSameFolderLevel);
			
			// Bind the command
			Commands->MapAction(
				FFUEditorCommands::Get().SelectSameFolderLevel,
				FExecuteAction::CreateRaw(this, &FFUEditor::ExecuteSelectSameFolderLevel)
			);
		}
	}
	
	// Level Editor actor operations
	{
		UToolMenu* ActorContextMenu = ToolMenus->ExtendMenu("LevelEditor.ActorContextMenu");
		
		FToolMenuSection* ActorOptionsSection = ActorContextMenu->FindSection("ActorOptions");
		
		FToolMenuEntry Entry(UToolMenus::Get()->CurrentOwner(), "CopyPasteOperationsSubMenu", EMultiBlockType::MenuEntry);
		Entry.TutorialHighlightName = NAME_None;
		Entry.Label = INVTEXT("Copy/Paste Operations");
		Entry.ToolTip = INVTEXT("");
		Entry.Icon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Transform");
		Entry.bShouldCloseWindowAfterMenuSelection = true;
		Entry.SubMenuData.bIsSubMenu = true;
		Entry.SubMenuData.ConstructMenu = FNewToolMenuDelegate::CreateStatic(&FFUEditor::FillActorOptionsFishyUtilsSubMenu);
		Entry.SubMenuData.bOpenSubMenuOnClick = false;
		Entry.InsertPosition = FToolMenuInsert("TransformSubMenu", EToolMenuInsertType::After);
		
		ActorOptionsSection->AddEntry(Entry);
		
		// Bind the commands
		Commands->MapAction(
			FFUEditorCommands::Get().ActorCopyTransform,
			FExecuteAction::CreateRaw(this, &FFUEditor::ExecuteActorCopyTransform)
		);
		Commands->MapAction(
			FFUEditorCommands::Get().ActorPasteTransform,
			FExecuteAction::CreateRaw(this, &FFUEditor::ExecuteActorPasteTransform),
			FCanExecuteAction::CreateRaw(this, &FFUEditor::CanExecuteActorPasteTransform)
		);
		
		Commands->MapAction(
			FFUEditorCommands::Get().ActorCopyLocation,
			FExecuteAction::CreateRaw(this, &FFUEditor::ExecuteActorCopyLocation)
		);
		Commands->MapAction(
			FFUEditorCommands::Get().ActorPasteLocation,
			FExecuteAction::CreateRaw(this, &FFUEditor::ExecuteActorPasteLocation),
			FCanExecuteAction::CreateRaw(this, &FFUEditor::CanExecuteActorPasteLocation)
		);
		
		Commands->MapAction(
			FFUEditorCommands::Get().ActorCopyRotation,
			FExecuteAction::CreateRaw(this, &FFUEditor::ExecuteActorCopyRotation)
		);
		Commands->MapAction(
			FFUEditorCommands::Get().ActorPasteRotation,
			FExecuteAction::CreateRaw(this, &FFUEditor::ExecuteActorPasteRotation),
			FCanExecuteAction::CreateRaw(this, &FFUEditor::CanExecuteActorPasteRotation)
		);
		
		Commands->MapAction(
			FFUEditorCommands::Get().ActorCopyScale,
			FExecuteAction::CreateRaw(this, &FFUEditor::ExecuteActorCopyScale)
		);
		Commands->MapAction(
			FFUEditorCommands::Get().ActorPasteScale,
			FExecuteAction::CreateRaw(this, &FFUEditor::ExecuteActorPasteScale),
			FCanExecuteAction::CreateRaw(this, &FFUEditor::CanExecuteActorPasteScale)
		);
	}
}

void FFUEditor::UnregisterMenuExtensions()
{
	UToolMenus::UnRegisterStartupCallback(this);
	
	// Unregister all our menu extensions
	UToolMenus::UnregisterOwner(this);
}

void FFUEditor::SetEditorGizmoTransform(const FTransform& NewTransform)
{
	auto* EditorViewportClient = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
	FEditorModeTools* ModeTools = EditorViewportClient->GetModeTools();
	
	if (ModeTools->AllowWidgetMove())
	{
		ModeTools->PivotLocation = NewTransform.GetLocation();
		ModeTools->SnappedLocation = NewTransform.GetLocation();
	}
	
	ModeTools->TranslateRotateXAxisAngle = NewTransform.Rotator().Yaw;
	ModeTools->TranslateRotate2DAngle = NewTransform.Rotator().Pitch;
}

void FFUEditor::ExecuteSelectSameFolderLevel()
{
	// check if not in PIE
	if (GWorld->IsPlayInEditor()) { return; }

	// get selected actors
	TArray<AActor*> SelectedActors;
	if (GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors) > 0)
	{
		AActor* ReferenceActor = SelectedActors[0];
		FFolder ReferenceFolder = ReferenceActor->GetFolder();

		for (TActorIterator<AActor> It(GWorld); It; ++It)
		{
			if (It->GetFolder() == ReferenceFolder)
			{
				GEditor->GetSelectedActors()->Select(*It);
			}
		}
	}
}

AActor* FFUEditor::GetSelectedActor() const
{
	TArray<AActor*> SelectedActors;
	GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);
	
	if (SelectedActors.IsEmpty())
	{
		return nullptr;
	}
	else if (SelectedActors.Num() > 1)
	{
		FNotificationInfo NotificationInfo(INVTEXT("Get Selected Actor"));
		NotificationInfo.ExpireDuration = 2;
		NotificationInfo.Image = FAppStyle::GetBrush("Icons.WarningWithColor");
		NotificationInfo.SubText = INVTEXT("Action only works with a single actor selected");
		FSlateNotificationManager::Get().AddNotification(NotificationInfo);
		return nullptr;
	}
	
	return  SelectedActors[0];
}

void FFUEditor::ExecuteActorCopyTransform()
{
	if (auto* SelectedActor = GetSelectedActor())
	{
		FString Value = SelectedActor->GetRootComponent()->GetComponentTransform().ToString();
		FPropertyEditorClipboard::ClipboardCopy(*Value);
	}
}

bool FFUEditor::CanExecuteActorPasteTransform() const
{
	FTransform Value;
	return GetPasteValueAs(Value);
}

void FFUEditor::ExecuteActorPasteTransform()
{
	if (auto* SelectedActor = GetSelectedActor())
	{
		FTransform Transform;
		if (GetPasteValueAs(Transform))
		{
			GEngine->BeginTransaction(TEXT("ExecuteActorPasteTransform"), INVTEXT("ExecuteActorPasteTransform"), nullptr);
			SelectedActor->Modify();
			SelectedActor->GetRootComponent()->SetWorldTransform(Transform);
			SetEditorGizmoTransform(Transform);
			GEngine->EndTransaction();
		}
	}
}

void FFUEditor::ExecuteActorCopyLocation()
{
	if (auto* SelectedActor = GetSelectedActor())
	{
		FString Value = SelectedActor->GetRootComponent()->GetComponentLocation().ToString();
		FPropertyEditorClipboard::ClipboardCopy(*Value);
	}
}

bool FFUEditor::CanExecuteActorPasteLocation() const
{
	FVector Value;
	return GetPasteValueAs(Value);
}

void FFUEditor::ExecuteActorPasteLocation()
{
	if (auto* SelectedActor = GetSelectedActor())
	{
		FVector Location;
		if (GetPasteValueAs(Location))
		{
			GEngine->BeginTransaction(TEXT("ExecuteActorPasteLocation"), INVTEXT("ExecuteActorPasteLocation"), nullptr);
			SelectedActor->Modify();
			SelectedActor->GetRootComponent()->SetWorldLocation(Location);
			SetEditorGizmoTransform(SelectedActor->GetRootComponent()->GetComponentTransform());
			GEngine->EndTransaction();
		}
	}
}

void FFUEditor::ExecuteActorCopyRotation()
{
	if (auto* SelectedActor = GetSelectedActor())
	{
		FString Value = SelectedActor->GetRootComponent()->GetComponentRotation().ToString();
		FPropertyEditorClipboard::ClipboardCopy(*Value);
	}
}

bool FFUEditor::CanExecuteActorPasteRotation() const
{
	FRotator Value;
	return GetPasteValueAs(Value);
}

void FFUEditor::ExecuteActorPasteRotation()
{
	if (auto* SelectedActor = GetSelectedActor())
	{
		FRotator Rotation;
		if (GetPasteValueAs(Rotation))
		{
			GEngine->BeginTransaction(TEXT("ExecuteActorPasteRotation"), INVTEXT("ExecuteActorPasteRotation"), nullptr);
			SelectedActor->Modify();
			SelectedActor->GetRootComponent()->SetWorldRotation(Rotation);
			GEngine->EndTransaction();
		}
	}
}

void FFUEditor::ExecuteActorCopyScale()
{
	if (auto* SelectedActor = GetSelectedActor())
	{
		FString Value = SelectedActor->GetRootComponent()->GetComponentScale().ToString();
		FPropertyEditorClipboard::ClipboardCopy(*Value);
	}
}

bool FFUEditor::CanExecuteActorPasteScale() const
{
	FVector Value;
	return GetPasteValueAs(Value);
}

void FFUEditor::ExecuteActorPasteScale()
{
	if (auto* SelectedActor = GetSelectedActor())
	{
		FVector Scale;
		if (GetPasteValueAs(Scale))
		{
			GEngine->BeginTransaction(TEXT("ExecuteActorPasteScale"), INVTEXT("ExecuteActorPasteScale"), nullptr);
			SelectedActor->Modify();
			SelectedActor->GetRootComponent()->SetWorldScale3D(Scale);
			GEngine->EndTransaction();
		}
	}
}

void FFUEditor::FillActorOptionsFishyUtilsSubMenu(UToolMenu* Menu)
{
	auto& TransformSection = Menu->AddSection("Transform", INVTEXT("Transform"));
	TransformSection.AddMenuEntry(FFUEditorCommands::Get().ActorCopyTransform, TAttribute<FText>(), TAttribute<FText>(), FGenericCommands::Get().Copy->GetIcon());
	TransformSection.AddMenuEntry(FFUEditorCommands::Get().ActorPasteTransform, TAttribute<FText>(), TAttribute<FText>(), FGenericCommands::Get().Paste->GetIcon());
	
	auto& LocationSection = Menu->AddSection("Location", INVTEXT("Location"));
	LocationSection.AddMenuEntry(FFUEditorCommands::Get().ActorCopyLocation, TAttribute<FText>(), TAttribute<FText>(), FGenericCommands::Get().Copy->GetIcon());
	LocationSection.AddMenuEntry(FFUEditorCommands::Get().ActorPasteLocation, TAttribute<FText>(), TAttribute<FText>(), FGenericCommands::Get().Paste->GetIcon());
	
	auto& RotationSection = Menu->AddSection("Rotation", INVTEXT("Rotation"));
	RotationSection.AddMenuEntry(FFUEditorCommands::Get().ActorCopyRotation, TAttribute<FText>(), TAttribute<FText>(), FGenericCommands::Get().Copy->GetIcon());
	RotationSection.AddMenuEntry(FFUEditorCommands::Get().ActorPasteRotation, TAttribute<FText>(), TAttribute<FText>(), FGenericCommands::Get().Paste->GetIcon());
	
	auto& ScaleSection = Menu->AddSection("Scale", INVTEXT("Scale"));
	ScaleSection.AddMenuEntry(FFUEditorCommands::Get().ActorCopyScale, TAttribute<FText>(), TAttribute<FText>(), FGenericCommands::Get().Copy->GetIcon());
	ScaleSection.AddMenuEntry(FFUEditorCommands::Get().ActorPasteScale, TAttribute<FText>(), TAttribute<FText>(), FGenericCommands::Get().Paste->GetIcon());
}

void FFUEditor::FFUEditorCommands::RegisterCommands()
{
	UI_COMMAND(SelectSameFolderLevel,
		"Select All In Folder",
		"Selects all other actors that are in the same root folder than the current selection (if multiple actors are selected the first selection will be used as reference)",
		EUserInterfaceActionType::Button, FInputChord(EKeys::S, EModifierKey::Shift | EModifierKey::Alt)
	);
	
	UI_COMMAND(ActorCopyTransform,
		"Copy Actor Transform", "Copy the transform of the selected actor", EUserInterfaceActionType::Button, FInputChord()
	);
	UI_COMMAND(ActorPasteTransform,
		"Paste Actor Transform", "Paste the cached transform on the selected actor", EUserInterfaceActionType::Button, FInputChord()
	);
	
	UI_COMMAND(ActorCopyLocation,
		"Copy Actor Location", "Copy the location of the selected actor", EUserInterfaceActionType::Button, FInputChord()
	);
	UI_COMMAND(ActorPasteLocation,
		"Paste Actor Location", "Paste the cached location on the selected actor", EUserInterfaceActionType::Button, FInputChord()
	);
	
	UI_COMMAND(ActorCopyRotation,
		"Copy Actor Rotation", "Copy the rotation of the selected actor", EUserInterfaceActionType::Button, FInputChord()
	);
	UI_COMMAND(ActorPasteRotation,
		"Paste Actor Rotation", "Paste the cached rotation on the selected actor", EUserInterfaceActionType::Button, FInputChord()
	);
	
	UI_COMMAND(ActorCopyScale,
		"Copy Actor Scale", "Copy the scale of the selected actor", EUserInterfaceActionType::Button, FInputChord()
	);
	UI_COMMAND(ActorPasteScale,
		"Paste Actor Scale", "Paste the cached scaled on the selected actor", EUserInterfaceActionType::Button, FInputChord()
	);
}

#undef LOCTEXT_NAMESPACE
