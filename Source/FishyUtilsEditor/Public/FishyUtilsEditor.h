// Fill out your copyright notice in the Description page of Project Settings.

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
