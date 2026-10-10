// By hzFishy - 2026 - Do whatever you want with it.

#include "FishyUtils.h"
#include "Debug/DebugDrawService.h"
#include "Draw/FUCanvasDraw.h"
#include "Framework/Commands/Commands.h"
#include "Utility/FUVisualLogger.h"


inline const TCHAR* TargetShowFlagName = TEXT("FU_CanvasDraw");  
inline TCustomShowFlag<EShowFlagShippingValue::Dynamic> TargetShowFlag(TargetShowFlagName, true, SFG_Normal, INVTEXT("FishyUtils: Canvas Draw"));
 

#define LOCTEXT_NAMESPACE "FFishyUtilsModule"

void FFishyUtilsModule::StartupModule()
{
#if ENABLE_VISUAL_LOG
	FFUVisualLoggerManager::Initialize();
#endif
	
	DebugDrawServiceDelegateHandle = UDebugDrawService::Register(TargetShowFlagName, FDebugDrawDelegate::CreateStatic(&FFUCanvasDrawService::DebugDraw_Global));
}

void FFishyUtilsModule::ShutdownModule()
{
#if ENABLE_VISUAL_LOG
	FFUVisualLoggerManager::Deinitialize();
#endif
	
	UDebugDrawService::Unregister(DebugDrawServiceDelegateHandle);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFishyUtilsModule, FishyUtils)