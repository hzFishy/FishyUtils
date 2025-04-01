// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FishyUtils/Private/Logging/dbgLog/dbgLog.h"


/*
#define BPG_LOG_Object(CATEGORY, VERBOSITY, THIS, FORMAT, ...) \
	UE_LOG(CATEGORY, VERBOSITY, TEXT("%s%s " FORMAT), *BPG_Logging::GetFunctionName(__FUNCTIONW__), *BPG_Logging::GetObjectDetailedName(THIS), ##__VA_ARGS__);
	*/

	/*----------------------------------------------------------------------------
		REQUIREMENTS

		You must give the Log verbosity to default and compile time level, example: DECLARE_LOG_CATEGORY_EXTERN(LogBPGValidator, Log, Log);
	----------------------------------------------------------------------------*/
	

	/*----------------------------------------------------------------------------
	   Core macros
	----------------------------------------------------------------------------*/

/*
#define _FU_LOG_Static(CATEGORY, VERBOSITY, FORMAT, ...) \
	UE_LOG(CATEGORY, VERBOSITY, TEXT("%s" FORMAT), *BPG_Logging::GetFunctionName(__FUNCTIONW__), ##__VA_ARGS__);
	*/

#define _FU_LOG_Static(CATEGORY, VERBOSITY, FORMAT, ...) do\
{\
	DBG::Log::Log(__COUNTER__, std::source_location::current(), CATEGORY, DBG::Log::DbgLogArgs{}.Verbosity(VERBOSITY).LogSourceLoc(), TEXT(FORMAT) __VA_OPT__(,) __VA_ARGS__);\
}while(false)


	/*----------------------------------------------------------------------------
	   Validation logging
	----------------------------------------------------------------------------*/
//#define FU_LOG_Validator_E(VERBOSITY, FORMAT, ...) BPG_LOG_Object(VERBOSITY, Error, this, FORMAT, ##__VA_ARGS__)
// static version
#define FU_LOG_SValidator_E(CATEGORY, FORMAT, ...) _FU_LOG_Static(CATEGORY, ELogVerbosity::Error, FORMAT, ##__VA_ARGS__)


namespace FU_Logging
{
	
}
