// By hzFishy - 2026 - Do whatever you want with it.


#include "Utility/FUVisualLogger.h"
#if ENABLE_VISUAL_LOG
#include "LogVisualizerSettings.h"
#endif


const UFUVisualLoggerSettings* UFUVisualLoggerSettings::Get()
{
	return GetDefault<UFUVisualLoggerSettings>();
}

FName UFUVisualLoggerSettings::GetCategoryName() const
{
	return "Plugins";
}

#if ENABLE_VISUAL_LOG
TSharedPtr<FFUVisualLoggerManager> FFUVisualLoggerManager::Instance;

void FFUVisualLoggerManager::Initialize()
{
	Instance = MakeShared<FFUVisualLoggerManager>();
	// No API export
	//Instance->OnFilterCategoryAddedHandle = FVisualLoggerFilters::Get().OnFilterCategoryAdded.AddSP(Instance.Get(), &FFUVisualLoggerManager::OnFilterCategoryAdded);
}

void FFUVisualLoggerManager::Deinitialize()
{
	// No API export
	//FVisualLoggerFilters::Get().OnFilterCategoryAdded.Remove(Instance->OnFilterCategoryAddedHandle);
	Instance.Reset();
}

void FFUVisualLoggerManager::OnFilterCategoryAdded(FString Name, ELogVerbosity::Type Verbosity)
{
	if (UFUVisualLoggerSettings::Get()->CategoriesToIgnore.Contains(Name))
	{
		// No API export
		//FVisualLoggerFilters::Get().RemoveCategory(Name);
	}
}
#endif
