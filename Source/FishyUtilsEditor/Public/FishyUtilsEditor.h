// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Utility/FUEditorUtilities.h"

class FFishyUtilsEditorModule : public IModuleInterface
{
protected:
    FFUEditor FishyEditorInstance = FFUEditor();
    
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
