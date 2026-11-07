// By hzFishy - 2026 - Do whatever you want with it.

#include "Slate/FUSlateIM.h"


namespace FU::SlateIM
{
#if UE_VERSION_NEWER_THAN_OR_EQUAL(5, 8, 0)
	FFUNomadTab::FFUNomadTab(FString Command, FString CommandHelp, FString TabTitle): 
		FSlateIMNomadTabBase(*TabTitle, *Command, *CommandHelp)
	{}

	FFUNomadTab::FFUNomadTab(FString Command, FString CommandHelp, FString TabTitle, FSlateIcon TabIcon): 
		FSlateIMNomadTabBase(*TabTitle, *Command, *CommandHelp, TabIcon)
	{}
#endif
}
