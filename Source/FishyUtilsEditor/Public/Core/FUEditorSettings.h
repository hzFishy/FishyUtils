// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "Utility/FUDeveloperSettings.h"
#include "FUEditorSettings.generated.h"


UCLASS(Config=EditorPerProjectUserSettings, DefaultConfig, DisplayName="Fishy Utils Editor Settings")
class FISHYUTILSEDITOR_API UFUEditorSettings : public UFUDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UFUEditorSettings();
	
	
	/** Need to restart editor */
	UPROPERTY(Config, EditAnywhere, Category="Misc", meta=(ConfigRestartRequired=true))
	bool bEnableMirrorPreview;
};
