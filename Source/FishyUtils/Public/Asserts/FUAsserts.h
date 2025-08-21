// By hzFishy 2025 - Do whatever you want with it

#pragma once


// taken from AssertionMacros.h (see definition of always)
#if DO_ENSURE && !USING_CODE_ANALYSIS

	#define FU_ALWAYS(InExpression)								ensureAlways(InExpression)
	#define FU_ALWAYSMSG(InExpression, InFormat, ...)			ensureAlwaysMsgf(InExpression, TEXT(InFormat), ##__VA_ARGS__)

#else

	#define FU_ALWAYS(InExpression)								(LIKELY(!!(InExpression)))
	#define FU_ALWAYSMSG(InExpression, InFormat, ...)			(LIKELY(!!(InExpression)))

#endif

	#define FU_ALWAYSVALID(Object)								FU_ALWAYS(IsValid(Object))
	#define FU_ALWAYSVALIDMSG(Object, InFormat, ...)			FU_ALWAYSMSG(IsValid(Object), InFormat, ##__VA_ARGS__)
	#define FU_ALWAYSWEAKVALIDMSG(Object, InFormat, ...)		FU_ALWAYSMSG(Object.IsValid(), InFormat, ##__VA_ARGS__)
	#define FU_ALWAYSWEAKNULLMSG(Object, InFormat, ...)			FU_ALWAYSMSG(!Object.IsNull(), InFormat, ##__VA_ARGS__)
