// By hzFishy 2025 - Do whatever you want with it

#pragma once


// taken from AssertionMacros.h (see definition of always)
#if DO_ENSURE && !USING_CODE_ANALYSIS

	#define FU_ENSURE(InExpression)								ensureAlways(InExpression)
	#define FU_ENSURE_MSG(InExpression, InFormat, ...)			ensureAlwaysMsgf(InExpression, TEXT(InFormat), ##__VA_ARGS__)

#else

	#define FU_ENSURE(InExpression)								(LIKELY(!!(InExpression)))
	#define FU_ENSURE_MSG(InExpression, InFormat, ...)			(LIKELY(!!(InExpression)))

#endif

	#define FU_ENSURE_VALID(Object)								FU_ENSURE(IsValid(Object))
	#define FU_ENSURE_VALID_MSG(Object, InFormat, ...)			FU_ENSURE_MSG(IsValid(Object), InFormat, ##__VA_ARGS__)
	#define FU_ENSURE_WEAKVALID(Object)							FU_ENSURE(Object.IsValid())
	#define FU_ENSURE_WEAKVALID_MSG(Object, InFormat, ...)		FU_ENSURE_MSG(Object.IsValid(), InFormat, ##__VA_ARGS__)
	#define FU_ENSURE_WEAKNOTNULL(Object)						FU_ENSURE(!Object.IsNull())
	#define FU_ENSURE_WEAKNOTNULL_MSG(Object, InFormat, ...)	FU_ENSURE_MSG(!Object.IsNull(), InFormat, ##__VA_ARGS__)
