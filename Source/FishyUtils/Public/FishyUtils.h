// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorOnly/FUEditor.h"
#include "Modules/ModuleManager.h"

class FFishyUtilsModule : public IModuleInterface
{
	
#if WITH_EDITOR
	FFUEditor FishyEditorInstance = FFUEditor();
#endif
	
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
