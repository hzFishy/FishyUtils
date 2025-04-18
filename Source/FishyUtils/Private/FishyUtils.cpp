// Copyright Epic Games, Inc. All Rights Reserved.

#include "FishyUtils.h"
#include "Framework/Commands/Commands.h"
#include "EditorOnly/FUEditor.h"

#define LOCTEXT_NAMESPACE "FFishyUtilsModule"

void FFishyUtilsModule::StartupModule()
{
#if WITH_EDITOR
	FFUEditor::FFUEditorCommands::Register();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(&FishyEditorInstance, &FFUEditor::RegisterMenuExtensions));
#endif
}

void FFishyUtilsModule::ShutdownModule()
{
#if WITH_EDITOR
	FishyEditorInstance.UnregisterMenuExtensions();
	FFUEditor::FFUEditorCommands::Unregister();

#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFishyUtilsModule, FishyUtils)