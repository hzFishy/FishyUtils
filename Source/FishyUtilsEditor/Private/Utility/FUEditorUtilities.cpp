// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "Utility/FUEditorUtilities.h"

#include "EngineUtils.h"
#include "LevelEditor.h"
#include "Selection.h"

#define LOCTEXT_NAMESPACE "FFishyUtilsModule"

void FU_EditorUtilities::GetAllGenerateOverlapEventsComponents(FU::Utils::FFUMessageBuilder& Builder)
{
	Builder.Append("All info about UPrimitiveComponents with GenerateOverlapEvents enabled");
	
	auto& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	
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
					GetAllGenerateOverlapEventsComponentsImpl(Builder, Components);
				}
			}
		}
	}
}

void FU_EditorUtilities::GetAllGenerateOverlapEventsComponentsForWorld(FU::Utils::FFUMessageBuilder& Builder, UWorld* World)
{
	for (auto It = TActorIterator<AActor>(World, AActor::StaticClass()); It; ++It)
	{
		TArray<UPrimitiveComponent*> RawComponents;
		(*It)->GetComponents<UPrimitiveComponent>(RawComponents);
		
		if (!RawComponents.IsEmpty())
		{
			TArray<const UPrimitiveComponent*> ConstComponents;
			ConstComponents.Reserve(RawComponents.Num());
			
			for (UPrimitiveComponent* Component : RawComponents)
			{
				ConstComponents.Emplace(Component);
			}
			
			GetAllGenerateOverlapEventsComponentsImpl(Builder, ConstComponents);
		}
	}
}

void FU_EditorUtilities::GetAllGenerateOverlapEventsComponentsImpl(FU::Utils::FFUMessageBuilder& Builder, const TArray<const UPrimitiveComponent*>& Components)
{
	for (auto* Component : Components)
	{
		if (Component->GetGenerateOverlapEvents())
		{
			if (Component->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
			{
				Builder.NewLinef(TEXT("- [NoCollision] %s"), *FU::Utils::GetObjectDetailedName(Component));
			}
			else
			{
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
				
				if (bAnyOverlapResp)
				{
					Builder.NewLinef(TEXT("- [Overlap Response used] %s"), *FU::Utils::GetObjectDetailedName(Component));
				}
				else
				{
					Builder.NewLinef(TEXT("- [None] %s"), *FU::Utils::GetObjectDetailedName(Component));
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