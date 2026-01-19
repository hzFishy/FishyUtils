// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "FishyUtils/Private/Logging/dbgLog/dbgLog.h"
#include "Utility/FUUtilities.h"

FISHYUTILS_API DECLARE_LOG_CATEGORY_EXTERN(LogFUTemp, Log, All);


#define FU_WITH_LOGGING KEEP_DBG_LOG


	/*----------------------------------------------------------------------------
		REQUIREMENTS

		You must give the Log verbosity to default and compile time level, example: DECLARE_LOG_CATEGORY_EXTERN(LogBPGValidator, Log, All);
		You may experience compiling errors if you dont use "Log" as runtime level and "All" as compile level.
	----------------------------------------------------------------------------*/

#if FU_WITH_LOGGING

	/*----------------------------------------------------------------------------
		  Suffixs explained:
			  - _D -> Display     // Has custom color in log window, see _FU_LOG_SET_DISPLAY_COLOR
			  - _W -> Warning
			  - _E -> Error
			  - _S -> Success     // Has custom color in log window, see _FU_LOG_SET_SUCCESS_COLOR
	  ----------------------------------------------------------------------------*/

	/*----------------------------------------------------------------------------
	   Core macros
	----------------------------------------------------------------------------*/

// Change the value to change color for display logs in window
#define _FU_LOG_CLEAR_WARN_COLOR CLEAR_WARN_COLOR()
#define _FU_LOG_SET_DISPLAY_COLOR SET_WARN_COLOR(COLOR_CYAN)
#define _FU_LOG_SET_SUCCESS_COLOR SET_WARN_COLOR(COLOR_GREEN)

#define _FU_LOG_OBJECT(CATEGORY, VERBOSITY, FORMAT, ...) do\
{\
	DBG::Log::Log(__COUNTER__, std::source_location::current(), CATEGORY, DBG::Log::DbgLogArgs{}.Verbosity(ELogVerbosity::VERBOSITY).SetFunctionName(__FUNCTIONW__).SetObjectDetailedName(FU::Utils::GetObjectDetailedName(this)), TEXT(FORMAT) __VA_OPT__(,) __VA_ARGS__);\
}while(false);

#define _FU_LOG_STATIC(CATEGORY, VERBOSITY, FORMAT, ...) do\
{\
	DBG::Log::Log(__COUNTER__, std::source_location::current(), CATEGORY, DBG::Log::DbgLogArgs{}.Verbosity(ELogVerbosity::VERBOSITY).SetFunctionName(__FUNCTIONW__), TEXT(FORMAT) __VA_OPT__(,) __VA_ARGS__);\
}while(false);

#else

#define _FU_LOG_CLEAR_WARN_COLOR {}
#define _FU_LOG_SET_DISPLAY_COLOR {}
#define _FU_LOG_SET_SUCCESS_COLOR {}


#define _FU_LOG_OBJECT(CATEGORY, VERBOSITY, FORMAT, ...) {}
#define _FU_LOG_STATIC(CATEGORY, VERBOSITY, FORMAT, ...) {}

#endif


// the rest defined here so its always available for any build mode
#define _FU_LOG_OBJECT_D(CATEGORY, FORMAT, ...) _FU_LOG_SET_DISPLAY_COLOR _FU_LOG_OBJECT(CATEGORY, Display, FORMAT, ##__VA_ARGS__)	_FU_LOG_CLEAR_WARN_COLOR
#define _FU_LOG_OBJECT_W(CATEGORY, FORMAT, ...)							  _FU_LOG_OBJECT(CATEGORY, Warning, FORMAT, ##__VA_ARGS__)
#define _FU_LOG_OBJECT_E(CATEGORY, FORMAT, ...)							  _FU_LOG_OBJECT(CATEGORY, Error, FORMAT, ##__VA_ARGS__)
#define _FU_LOG_OBJECT_S(CATEGORY, FORMAT, ...) _FU_LOG_SET_SUCCESS_COLOR _FU_LOG_OBJECT(CATEGORY, Display, FORMAT, ##__VA_ARGS__)	_FU_LOG_CLEAR_WARN_COLOR

#define _FU_LOG_STATIC_D(CATEGORY, FORMAT, ...) _FU_LOG_SET_DISPLAY_COLOR _FU_LOG_STATIC(CATEGORY, Display, FORMAT, ##__VA_ARGS__)	_FU_LOG_CLEAR_WARN_COLOR
#define _FU_LOG_STATIC_W(CATEGORY, FORMAT, ...)							  _FU_LOG_STATIC(CATEGORY, Warning, FORMAT, ##__VA_ARGS__)
#define _FU_LOG_STATIC_E(CATEGORY, FORMAT, ...)							  _FU_LOG_STATIC(CATEGORY, Error, FORMAT, ##__VA_ARGS__)
#define _FU_LOG_STATIC_S(CATEGORY, FORMAT, ...) _FU_LOG_SET_SUCCESS_COLOR _FU_LOG_STATIC(CATEGORY, Display, FORMAT, ##__VA_ARGS__)	_FU_LOG_CLEAR_WARN_COLOR

#define FU_LOG_Temp_D(FORMAT, ...)										  _FU_LOG_OBJECT_D(LogFUTemp, FORMAT, ##__VA_ARGS__)
#define FU_LOG_Temp_W(FORMAT, ...)										  _FU_LOG_OBJECT_W(LogFUTemp, FORMAT, ##__VA_ARGS__)
#define FU_LOG_STemp_D(FORMAT, ...)										  _FU_LOG_STATIC_D(LogFUTemp, FORMAT, ##__VA_ARGS__)
#define FU_LOG_STemp_W(FORMAT, ...)										  _FU_LOG_STATIC_W(LogFUTemp, FORMAT, ##__VA_ARGS__)

#define FU_LOG_Validation_E(CATEGORY, FORMAT, ...)						  _FU_LOG_OBJECT_E(CATEGORY, FORMAT, ##__VA_ARGS__)
#define FU_LOG_SValidation_E(CATEGORY, FORMAT, ...)						  _FU_LOG_STATIC_E(CATEGORY, FORMAT, ##__VA_ARGS__)


#define FU_LOG_THIS *FU::Utils::GetObjectDetailedName(this)
