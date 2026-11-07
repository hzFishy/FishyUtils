// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "SlateIMWidgetBase.h"
#include "Misc/EngineVersionComparison.h"


namespace FU::SlateIM
{
#if UE_VERSION_NEWER_THAN_OR_EQUAL(5, 8, 0)
	class FFUNomadTab : public FSlateIMNomadTabBase
	{
	public:
		FISHYUTILS_API FFUNomadTab(FString Command, FString CommandHelp, FString TabTitle);
		FISHYUTILS_API FFUNomadTab(FString Command, FString CommandHelp, FString TabTitle, FSlateIcon TabIcon);
		
		/** Override this to draw your widget */
		virtual void DrawContent(float DeltaTime) override {};
	};
#endif
}
