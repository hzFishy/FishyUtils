﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorOnly/FUEditor.h"

#include "ActorFolderTreeItem.h"
#include "EngineUtils.h"
#include "FolderTreeItem.h"
#include "LevelEditor.h"
#include "SSceneOutliner.h"
#include "Editor/SceneOutliner/Public/ISceneOutliner.h"
#include "Logging/FULogging.h"


#if WITH_EDITOR

#define LOCTEXT_NAMESPACE "FFishyUtilsModule"

#include "Selection.h"

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
		INVTEXT("Fishy Utilities"),
		FToolMenuInsert("SelectBones", EToolMenuInsertType::After)
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
		"Selects all other actors that are in the samme root folder than the current selection (if multiple actors are selected the first selection will be used as reference)",
		EUserInterfaceActionType::Button, FInputChord(EKeys::S, EModifierKey::Shift | EModifierKey::Alt)
	);
}

#undef LOCTEXT_NAMESPACE

#endif
