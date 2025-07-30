// By hzFishy 2025 - Do whatever you want with it

#pragma once


// taken from AssertionMacros.h (see definition of always)
#if DO_ENSURE && !USING_CODE_ANALYSIS

	#define FU_ASRT_ALWAYS(InExpression)							ensureAlways(InExpression)
	#define FU_ASRT_ALWAYSMSG(InExpression, InFormat, ... )			ensureAlwaysMsgf(InExpression, TEXT(InFormat), ##__VA_ARGS__)

#else

	#define FU_ASRT_ALWAYS(InExpression)							(LIKELY(!!(InExpression)))
	#define FU_ASRT_ALWAYSMSG(InExpression, InFormat, ... )			(LIKELY(!!(InExpression)))

#endif
