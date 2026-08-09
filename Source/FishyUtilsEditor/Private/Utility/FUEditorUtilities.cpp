// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "Utility/FUEditorUtilities.h"
#include "EditorCategoryUtils.h"
#include "EngineUtils.h"
#include "LevelEditor.h"
#include "Selection.h"
#include "Misc/UObjectToken.h"

#define LOCTEXT_NAMESPACE "FFishyUtilsModule"


namespace FU_EditorUtilities
{
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
						->AddText(FText::FromString(FString::Printf(TEXT("[%s] %s - "), *GenerateOverlapEventsResultToString(Result), *FU::Utils::GetObjectDetailedName(Component))))
						->AddToken(FActorToken::Create(Actor->GetPathName(), Actor->GetActorGuid(), INVTEXT("Focus Actor")))
						->AddToken(FActionToken::Create(INVTEXT("Open Blueprint"), INVTEXT("Open Blueprint"), FOnActionTokenExecuted::CreateLambda([BPAsset] ()
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
				
				// TODO: fix
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
			->AddText(FText::FromString(FString::Printf(TEXT("[%s] %s - "), *GenerateOverlapEventsResultToString(Result), *FU::Utils::GetObjectDetailedName(Component))))
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
}


FFUEditor::FFUEditor() {}

FFUEditor::~FFUEditor() {}

void FFUEditor::RegisterMenuExtensions()
{
	// Use the current object as the owner of the menus
	// This allows us to remove all our custom menus when the module is unloaded
	FToolMenuOwnerScoped OwnerScoped(this);
 
	UToolMenu* SelectionMenu = UToolMenus::Get()->ExtendMenu(
		"LevelEditor.MainMenu.Select");
	
	FToolMenuSection& FishySelectionSection = SelectionMenu->AddSection(
		"FishyUtilities",
		INVTEXT("Fishy Utilities")
	);

	FishySelectionSection.AddMenuEntry(FFUEditorCommands::Get().SelectSameFolderLevel);

	// Bind the commands
	const FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	const TSharedRef<FUICommandList> Commands = LevelEditor.GetGlobalLevelEditorActions();
 
	Commands->MapAction(
		FFUEditorCommands::Get().SelectSameFolderLevel,
		FExecuteAction::CreateRaw(this, &FFUEditor::ExecuteSelectSameFolderLevel)
	);
}

void FFUEditor::UnregisterMenuExtensions()
{
	UToolMenus::UnRegisterStartupCallback(this);
 
	// Unregister all our menu extensions
	UToolMenus::UnregisterOwner(this);
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

void FFUEditor::FFUEditorCommands::RegisterCommands()
{
	UI_COMMAND(SelectSameFolderLevel,
		"Select All In Folder",
		"Selects all other actors that are in the same root folder than the current selection (if multiple actors are selected the first selection will be used as reference)",
		EUserInterfaceActionType::Button, FInputChord(EKeys::S, EModifierKey::Shift | EModifierKey::Alt)
	);
}

#undef LOCTEXT_NAMESPACE