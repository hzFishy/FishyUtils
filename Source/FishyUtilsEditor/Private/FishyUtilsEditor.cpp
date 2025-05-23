﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "FishyUtilsEditor.h"
#include "Utility/FUEditorUtilities.h"
#include "Utility/FUPickers.h"
#include "Utility/FUPickerSocketCustomization.h"


#define LOCTEXT_NAMESPACE "FFishyUtilsEditorModule"

void FFishyUtilsEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FFUPickerSockets::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FFUPickerSocketCustomization::MakeInstance)
	);

	FFUEditor::FFUEditorCommands::Register();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(&FishyEditorInstance, &FFUEditor::RegisterMenuExtensions));
	
}

void FFishyUtilsEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout(FFUPickerSockets::StaticStruct()->GetFName());

	FishyEditorInstance.UnregisterMenuExtensions();
	FFUEditor::FFUEditorCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FFishyUtilsEditorModule, FishyUtilsEditor)
