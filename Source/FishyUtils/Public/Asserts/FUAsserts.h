// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


// taken from AssertionMacros.h (see definition of always)
#if DO_ENSURE && !USING_CODE_ANALYSIS

	#define FU_ASRT_ALWAYS(InExpression)							ensureAlways(InExpression)
	#define FU_ASRT_ALWAYSMSG(InExpression, InFormat, ... )			ensureAlwaysMsgf(InExpression, TEXT(InFormat), ##__VA_ARGS__)

#else

	#define FU_ASRT_ALWAYS(InExpression)							{}
	#define FU_ASRT_ALWAYSMSG(InExpression, InFormat, ... )			{}

#endif