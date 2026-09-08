// By hzFishy - 2026 - Do whatever you want with it.

#pragma once
#include "FUConsole.h"
#include "Logging/FULogging.h"
#include "Utility/FUUtilities.h"


namespace FU::Utils::Command
{
	FU_CMD_RUNLAMBDA_WITHARGS(GenerateRandomHexStringCmd,
		"FU.Misc.GenerateRandomHexString", "Optional param: length of the generated unique id (default: 8, min: 2, max: 32)",
		{
			float Length = 8;
			
			if (!Args.IsEmpty())
			{
				Length = FCString::Atof(*Args[0]);
			}
			
			Length = FMath::Clamp(Length, 2, 32);
			const FString GeneratedString = FU::Utils::GenerateRandomHexString(Length);
			FU_LOG_STemp_W("GenerateRandomHexString result of length {0}: {1}", Length, GeneratedString);
		}
	);
}
