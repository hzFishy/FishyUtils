// By hzFishy - 2026 - Do whatever you want with it.

#include "Slate/FUSlateIM.h"


namespace FU::SlateIM
{
	FFUNomadTab::FFUNomadTab(FString Command, FString CommandHelp, FString TabTitle): 
		FSlateIMNomadTabBase(*TabTitle, *Command, *CommandHelp)
	{}

	FFUNomadTab::FFUNomadTab(FString Command, FString CommandHelp, FString TabTitle, FSlateIcon TabIcon): 
		FSlateIMNomadTabBase(*TabTitle, *Command, *CommandHelp, TabIcon)
	{}
}
