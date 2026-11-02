// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "SlateIMWidgetBase.h"


namespace FU::SlateIM
{
	class FFUNomadTab : public FSlateIMNomadTabBase
	{
	public:
		FISHYUTILS_API FFUNomadTab(FString Command, FString CommandHelp, FString TabTitle);
		FISHYUTILS_API FFUNomadTab(FString Command, FString CommandHelp, FString TabTitle, FSlateIcon TabIcon);
		
		/** Override this to draw your widget */
		virtual void DrawContent(float DeltaTime) override {};
	};
}
