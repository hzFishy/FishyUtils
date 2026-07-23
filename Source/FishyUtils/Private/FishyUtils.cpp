// By hzFishy - 2026 - Do whatever you want with it.

#include "FishyUtils.h"
#include "Framework/Commands/Commands.h"
#include "Utility/FUVisualLogger.h"

#define LOCTEXT_NAMESPACE "FFishyUtilsModule"

void FFishyUtilsModule::StartupModule()
{
#if ENABLE_VISUAL_LOG
	FFUVisualLoggerManager::Initialize();
#endif
}

void FFishyUtilsModule::ShutdownModule()
{
#if ENABLE_VISUAL_LOG
	FFUVisualLoggerManager::Deinitialize();
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFishyUtilsModule, FishyUtils)