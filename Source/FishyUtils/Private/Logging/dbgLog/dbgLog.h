// Copyright ©  Jack Holland.
//
// DbgLOG is free software. It comes without any warranty, to the extent permitted
// by applicable law. You can redistribute it and/or modify it under the terms
// of the Do What The Fuck You Want To Public License, Version 2, as published
// by Sam Hocevar:
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
// Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
//
// Everyone is permitted to copy and distribute verbatim or modified
// copies of this license document, and changing it is allowed as long
// as the name is changed.
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.
#pragma once

#include "Framework/Notifications/NotificationManager.h"

#include "FishyUtils/Private/Logging/dbgLog/LLog.h"
#include "VisualLogger/VisualLogger.h"
#include "Misc/EngineVersionComparison.h"

#include "Widgets/Notifications/SNotificationList.h"
#include "dbgLog.generated.h"

/**
 *	This library is built on top of LLog - https://github.com/landelare/llog and adds some QOL features
 *	(like logging compile-time arrays, not crashing on null pointers and a few other things) as well as implementing
 *	the actual macros and Builder pattern for logging (more on this later). But a HUGE shout out goes
 *	to Laura for her work and logging articles https://landelare.github.io/2022/04/28/better-ue_log.html
 *
 *
 *	Simple overview of the usage of this logging header utility library.
 *	This library is built off of the `std::format` and has better formatting support than standard
 *	FString::Format (which doesn't even let you specify arg placements or decimal places) and is a LOT
 *	safer than Printf formats which crash by simply passing the wrong % specifier at runtime.
 *
 *	Here we compile-time assert if anything is not correct or would cause a crash.
 *	Please See https://en.cppreference.com/w/cpp/utility/format/spec for a more in-depth look at formatting options
 *	which are supported here.
 *
 *	
 *	There are really only two macros to concern yourself with - `dbgLOG` and `dbgLOGV`.
 *
 *	dbgLOG is your standard `std::format` log macro that takes a format message and optional logs, some example usages are as follows:
 *
 *	- dbgLOG( "No args, regular log" );												// Outputs: "No args, regular log"
 *	- dbgLOG( "Mr Tim {0}", "Sweeney" );											// Outputs: "Mr Tim Sweeney"
 *	- dbgLOG( "{1}{0}", "Bar", "Foo" );												// Outputs: "Foo Bar" (see how we can reorder the arguments depending on their placement)
 *	- dbgLOG( "{1}{0} {2}", "Bar", "Foo", 42 );										// Outputs: "Foo Bar 42"
 *	- dbgLOG( "{0:.3f}", 3.14159265 );												// Outputs: "3.142" (see how we specify :.3f to output only up to three decimal places) 
 *	- dbgLOG( "{0}", 3.141592653589793238 );										// Outputs: "3.141592653589793238" (outputs the entire value)
 *	- dbgLOG( "{0:.3f}, {1:.5f}",	3.141592653589793238, 6.283185307179586476 );	// Outputs: "3.142, 6.28319"
 *
 *	We can also log containers(such as; TArray, TMap, Tuples, C style arrays), enums, structs, pointers etc in a native fashion. Containers are recursive and call the same format function
 *	for each of their elements
 *		
 *	ESomeOtherEnum SomeOtherEnum = ESomeOtherEnum::Val_3;
 *	dbgLOG( "Reflected enum: {0}", SomeOtherEnum );				// Outputs: "Reflected enum: ESomeOtherEnum::Val_3", if the enum is not using reflection it would just output its constant value as an int
 *
 *
 *	If a struct type has a ToString or GetName function, that will be preferred BUT if it doesn't and the struct *is* reflected
 *	we can still log its contents by falling back to unreals `ExportText` which prints all reflected variables and their values, for example
 *	
 *	USTRUCT(BlueprintType)
 *	struct FMyReflectedStruct
 *	{
 *		GENERATED_BODY()
 *	public:
 *		UPROPERTY(EditAnywhere)
 *		FString SomeStr = TEXT("Foo");
 *	
 *		UPROPERTY(EditAnywhere)
 *		int SomeInt = 42;
 *		
 *		UPROPERTY(EditAnywhere)
 *		float SomeFloat = 3.14f;
 *	};
 *
 *	FMyReflectedStruct S{};
 *	dbgLOG( "Reflected Struct: {0}", S );	// Outputs: "Reflected Struct: (SomeStr="Foo",SomeInt=42,SomeFloat=3.14)"
 *
 *	If we couldn't find a way to log the type then we get a compile-time error.
 *
 *	Here are some more examples
 *	
 * 	UMyObject* ThisPointer = this;
 * 	dbgLOG( "UObject Pointer: {0}", this );									// Outputs: "UObject Pointer: (UMyObject*)Name"
 * 
 * 	ThisPointer = nullptr;
 * 	dbgLOG( "UObject Pointer: {0}", ThisPointer );							// Outputs: "UObject Pointer: (MyType*)Null Ptr", see how null won't crash the program.
 * 
 * 
 * 	int MyStaticArray[5] = {0, 1, 2, 3, 4};
 * 	dbgLOG( "Static sized Array: {0}", MyStaticArray );						// Outputs: "Static sized Array: [0, 1, 2, 3, 4]"
 * 
 * 
 * 	TArray<int> MyDynamicArray = {5, 6, 7, 8, 9};
 * 	dbgLOG( "Dynamic Array: {0}", MyDynamicArray );							// Outputs: "Dynamic Array: [5, 6, 7, 8, 9]"
 *
 *
 *  Not reflected but we can log it because we defined a ToString
 * 	struct FHealth
 *	{
 *		FString ToString() const {return FString::Format(TEXT("Health: {0}"), {Health}); }
 *		float Health = 95.f;
 *	};
 *	
 * 	TMap<int, FHealth> SomeStructMap = {
 * 		{0, {95.f}},
 * 		{1, {85.f}},
 * 		{2, {75.f}},
 * 		{3, {65.f}},
 * 		{4, {55.f}}
 * 	};
 * 	dbgLOG( "Struct Map: {0}", SomeStructMap );								// Outputs: "Struct Map: [ {0, Health: 95}, {1, Health: 85}, {2, Health: 75}, {3, Health: 65}, {4, Health: 55} ]"
 * 
 *
 *
 * 
 *	---------------------------------------- Verbose log -----------------------------------------
 * 
 *	`dbgLOGV` is just an extension of `dbgLOG` with the added possibility for builder args, if you are not familiar with this concept it basically
 *	just allows you to chain arguments together, for example ` .ThingOne(Value).ThingTwo(Value).ThingThree(Value)` and it works because each chained
 *	argument just returns a reference to the original builder object.
 *
 *	This style of a log macro allows for basically multiple variadic args (the builder and the format args).
 *
 *	Here is a simple example of how we set the verbosity of our log
 *	dbgLOGV( .Verbosity( ELogVerbosity::Warning ), "No args, regular log");
 *
 *
 *	Here we choose to make this log output to both screen and console (the options are `Con, Scr, Both`) Note how we chain multiple args
 *	dbgLOGV( .Verbosity( ELogVerbosity::Error ).Output( Both ),
 *			"{0}", 3.14 );
 *
 *
 *
 *	WCO gives more contextual information in the output in the format of `[Client | Instance: 0]: My value is - whatever`
 *	where Client could be "Dedicated Server", "Standalone", etc.
 *	Also we can runtime log with *whatever* category name we want (the only thing of note is we prefix all categories with "dbg" to ensure
 *	we have no runtime clashes with existing categories set by the engine)
 *	
 *	dbgLOGV( .Category( "MyCustomCategory" ).WCO( this ),
 *		"My value is - {0}", GetSomeValue);
 *
 *
 *
 *	`Condition` is great for ensuring conditions and branches for logging are not actually kept in shipping builds (this is my favorite)
 *	dbgLOGV( .Condition( false ), "I only log if the condition is true {0}", GetWorld()->GetTimeSeconds() );
 *
 *
 *
 *  Visual logger and Debug shapes are also supported as Builder args, examples below:
 *  
 *  Draws this actor's bounds into the visual logger (the reason for this, this is because
 *  it takes the log owner and the actor to draw which can be different in certain circumstances)
 *  We can only have one type of visual log per macro though.
 *  
 *	dbgLOGV( .VisualLogBounds( this, this ),
 *		"Visual Log Test {0}", GetWorld()->GetTimeSeconds());
 *
 *		
 *	Many different DrawDebugShape functions are supported to ensure they are compiled out of shipping builds and the usage of this library is streamlined
 *	dbgLOGV( .DrawDebugSphere( this, GetActorLocation(), 50.f, 12 ), "Drawing Sphere...");
 */










UENUM(BlueprintType)
enum EDbgLogOutput : uint8
{
	// Outputs only to the console
	Con = 0,

	// Outputs only the the screen (if possible, otherwise falls back to the console)
	Scr,
	
	// Outputs to both the screen (if possible) and console.
	Both
};

#define KEEP_DBG_LOG (!UE_BUILD_SHIPPING) || (USE_LOGGING_IN_SHIPPING) 


#if KEEP_DBG_LOG

#define _INTERNAL_DBGLOGV(LogCategory, Args, Msg, ...) do\
{\
    DBG::Log::DbgLogArgs Name{}; \
    Name Args;\
    DBG::Log::Log(__COUNTER__, std::source_location::current(), LogCategory,  TEXT(Msg) __VA_OPT__(,) __VA_ARGS__); \
} while(false)



/**
 *	This your standard formatted log message, could either be `"Hello World" or "Hello World {0}" where 0 is placement
 * for your log arguments, for example:
 * \code
 * // This takes no args.
 * dbgLOG( "Hello World" );
 * \endcode
 * \code
 * // This takes any amount of args.
 * dbgLOG( "Hello World {0}", 42 );
 * \endcode
 * \code
 * // This is how you would format a float/double to only show 3 places - similar to printf's `%.3f`
 * dbgLOG( "Hello World {0:.3f}", 3.1415926535 );
 * \endcode
 */
#define dbgLOG(LogCategory, Msg, ...) do\
{\
    DBG::Log::Log(__COUNTER__, std::source_location::current(), LogCategory, DBG::Log::DbgLogArgs{}, TEXT(Msg) __VA_OPT__(,) __VA_ARGS__);\
}while(false)


/**
 * This a slightly more advanced log where you have the ability to set extra information about the log, such as its verbosity, where it outputs (screen or console) and a lot more.
 * This log is more less the same as the standard log but with one param at the beginning that begins with the `.` operator to access the log args object.
 * For example:
 *
 * \code
 * // This outputs our log to only the screen but Scr could have also been `Con` or `Both`
 * 
 * dbgLOGV( .Output(Scr), "Hello World" ); 
 * \endcode
 * \code
 * // This outputs as long as the condition is true, the input could be some function like
 * // `HasAmmo()` and it only logs if we have ammo.
 * 
 * dbgLOGV( .Condition( true ), "Hello World" );
 * \endcode
 * \code
 * // Lastly you can chain these args like so.
 * // This prints "Hello World 42" along with the source location of the macro
 * // (file, line number and function it was called from),
 * // to the console with a yellow warning verbosity and has the runtime category "dbgPlayer"
 * 
 * dbgLOGV( .LogSourceLoc().Output( Con ).Verbosity( ELogVerbosity::Warning ).Category( "Player" ) ,
 *	"Hello World {0}", 42 );
 * \endcode
 */
#define dbgLOGV(LogCategory, Args, Msg, ...) _INTERNAL_DBGLOGV(LogCategory, Args, Msg, _CONCAT(LogArgs, __COUNTER__) __VA_OPT__(,) __VA_ARGS__)





namespace DBG::Log
{
	struct DbgLogArgs
	{
		
	private:
		enum struct EDbgVisualLogShape : uint8
		{
			None,
			Sphere,
			Box,
			Cone,
			Line,
			Arrow,
			Disk,
			Capsule,
		};

	public:
		using ThisClass = DbgLogArgs;
		template<typename... A>
		friend void Log(int32 UniqueIdentifier, std::source_location Location,
				const FLogCategoryBase& Category,
				DbgLogArgs LogArgs, 
				std::wformat_string<TFormatted<A>...> Format, 
				A&&... Args);
		
		DbgLogArgs() = default;

		// The log category, can be anything you want but `dbg` is appended due to possible naming conflicts with existing categories (which can cause an asset).
		ThisClass& Category(FName CategoryName) { CategoryNameValue = CategoryName; return *this; }
		
		// Verbosity is the level of the log message, usage is `.Verbosity( ELogVerbosity::Warning )` etc.
		ThisClass& Verbosity(ELogVerbosity::Type Verb) { VerbosityValue = Verb; return *this; }

		// World context Object, can be used to append to the log the PIE ID as well as net mode.
		ThisClass& WCO(const UObject* CO) { WCOResultValue = GEngine->GetWorldFromContextObject( CO, EGetWorldErrorMode::ReturnNull ); return *this; }
		
		// Where the message is output, Con, Scr, or Both.
		ThisClass& Output(EDbgLogOutput Out) { OutputDestinationValue = Out; return *this; }
		
		// Appended to the log message as `[PREFIX]: Regular Log Msg`.
		ThisClass& Prefix(const FString& PrefixStr) { PrefixValue = PrefixStr; return *this; }
		
		// Color for screen logs.
		ThisClass& ScrnColor(FColor Col) { ScreenColorValue = Col; return *this; }
		
		// Duration for screen logs.
		ThisClass& ScrnDuration(float Duration) { ScreenDurationValue = Duration; return *this; }
		
		// Optional key for screen logs.
		ThisClass& ScrnKey(int32 Key) { ScreenKeyValue.Emplace( Key ) ; return *this; }
		
		// Condition for the log, great for things like CVars or only logging under certain conditions.
		ThisClass& Condition(bool Condition) { bLogConditionValue = Condition; return *this; }

		// If called, it enables logging of the source location, things like file name, line number etc.
		ThisClass& LogSourceLoc() { bLogSourceLocation = true; return *this; }

		// If called, enables the default format for the date and time which is prepended to the log.
		ThisClass& LogDateAndTime() { bLogDateAndTime = true; return *this; }

		/**
		 * Lets this macro log to a slate notification popup in the bottom corner
		 * @param bOnlyUseThisLog If true we do not attempt to also log to the console or screen.
		 */
		ThisClass& LogToSlateNotify(bool bOnlyUseThisLog = false)
		{
			bOnlyLogToSlateNotify = bOnlyUseThisLog;
			bLogToSlateNotify = true;
			return *this;
		}

		/**
		 * Logs the provided message to a dialog box the user can interact with.
		 * @param Response User response to the dialog
		 * @param MsgType What type of dialog should be shown
		 * @param bOnlyUseThisLog If true we do not attempt to also log to the console or screen.
		 */
		ThisClass& LogToMessageDialog(const TFunction<void(EAppReturnType::Type UserResponse)>& Response,
			EAppMsgType::Type MsgType, bool bOnlyUseThisLog = false)
		{
			AppMessageResponse = Response;
			AppMsgType = MsgType;
			bOnlyLogToMessageDialog = bOnlyUseThisLog;
			bLogToMessageDialog = true;
			return *this;
		}
		
		/** 
		 * Logs a message to the editor message log (this is a window that pops up showing you any errors or warnings)
		 * You would see this when accessing null in a BP graph for example after you quit PIE.
		 * @param bShouldShowEditorMessageLogImmediately If true we instantly spawn the editor message log window,
		 * otherwise you need to manually check the window
		 */
		ThisClass& LogToEditorMessageLog(bool bShouldShowEditorMessageLogImmediately = false)
		{
			bLogToEditorMessageLog = true;
			bShowEditorMessageLogImmediately = bShouldShowEditorMessageLogImmediately;
			return *this;
		}
		
		/**
		 * Logs the date and time with the provided format
		 * %y = Year YY, %Y = Year YYYY, %m = Month 01-12, %d = Day 01-31
		 * %h = Hour 0-12, %H = Hour 00-23, %M = Minute 00-59, %S = Second 00-59, %s = Millisecond 000-999
		 * Example `.LogDateAndTime( TEXT( "%d/%m/%y %H:%M:%S" ) )` outputs `03/04/2025 06:42:29`
		 */
		ThisClass& LogDateAndTime(TStringView<TCHAR> Format) { bLogDateAndTime = true; DateTimeFormat = Format; return *this; }


		ThisClass& DrawDebugCapsule(const UObject* WorldContextObject,
			const FVector& Center,
			float HalfHeight,
			float Radius,
			const FQuat& Rotation,
			const FColor& Color,
			bool bPersistentLines = false,
			float LifeTime = -1.f,
			uint8 DepthPriority = 0,
			float Thickness = 0);

		ThisClass& DrawDebugCone(const UObject* WorldContextObject,
			const FVector& Origin,
			const FVector& Direction,
			float Length,
			float AngleWidth,
			float AngleHeight,
			int32 NumSides,
			const FColor& Color = FColor::Orange,
			bool bPersistentLines = false,
			float LifeTime = -1.f,
			uint8 DepthPriority = 0,
			float Thickness = 0);

		ThisClass& DrawDebugCylinder(const UObject* WorldContextObject,
			const FVector& Start,
			const FVector& End,
			float Radius,
			int32 Segments,
			const FColor& Color = FColor::Orange,
			bool bPersistentLines = false,
			float LifeTime = -1.f,
			uint8 DepthPriority = 0,
			float Thickness = 0);

		ThisClass& DrawDebugDirectionalArrow(const UObject* WorldContextObject,
			const FVector& LineStart,
			const FVector& LineEnd,
			float ArrowSize,
			const FColor& Color = FColor::Orange,
			bool bPersistentLines = false,
			float LifeTime = -1.f,
			uint8 DepthPriority = 0,
			float Thickness = 0);

		ThisClass& DrawDebugLine(const UObject* WorldContextObject,
			const FVector& LineStart,
			const FVector& LineEnd,
			const FColor& Color = FColor::Orange,
			bool bPersistentLines = false,
			float LifeTime = -1.f,
			uint8 DepthPriority = 0, float Thickness = 0);

		ThisClass& DrawDebugPoint(const UObject* WorldContextObject,
			const FVector& Position,
			float Size,
			const FColor& Color = FColor::Orange,
			bool bPersistentLines = false,
			float LifeTime = -1.f,
			uint8 DepthPriority = 0);

		ThisClass& DrawDebugSphere(const UObject* WorldContextObject,
			const FVector& Center,
			float Radius,
			int32 Segments,
			const FColor& Color = FColor::Orange,
			bool bPersistentLines = false,
			float LifeTime = -1.f,
			uint8 DepthPriority = 0,
			float Thickness = 0);

		ThisClass& DrawDebugString(const UObject* WorldContextObject,
			const FVector& TextLocation,
			const FString& Text,
			AActor* TestBaseActor = nullptr,
			const FColor& TextColor = FColor::Orange,
			float Duration = -1.f);

		ThisClass& DrawDebugBox(const UObject* WorldContextObject,
			const FVector& Center,
			const FVector& Extent,
			const FColor& Color = FColor::Orange,
			bool bPersistentLines = false,
			float LifeTime = -1.f,
			uint8 DepthPriority = 0, float Thickness = 0);


		// you may only use one visual log per macro.

		// Logs text with the visual logger system.
		DbgLogArgs& VisualLogText(UObject* Owner,
			bool bOnlyLogVisually = true);

		// Logs a sphere with the visual logger system.
		DbgLogArgs& VisualLogSphere(UObject* Owner,
			const FVector& Location, float Radius,
			bool bDrawWireframe = false,
			bool bOnlyLogVisually = true);

		// Logs a box with the visual logger system.
		DbgLogArgs& VisualLogBox(UObject* Owner,
			const FVector& MinExtent,
			const FVector& MaxExtent,
			const FVector& Location,
			const FRotator& Rotation = FRotator::ZeroRotator,
			FColor BoxColor = FColor::Orange,
			bool bDrawWireframe = false,
			bool bOnlyLogVisually = true);
		
		// Logs the provided actors bounds into the visual logger system.
		DbgLogArgs& VisualLogBounds(UObject* Owner,
			const AActor* ActorToGetBoundsFrom,
			FColor BoundsColor = FColor::Orange,
			bool bDrawWireframe = false,
			bool bOnlyLogVisually = true);

		// Logs a cone with the visual logger system.
		DbgLogArgs& VisualLogCone(UObject* Owner,
			const FVector& Location,
			const FVector& Direction,
			float Length, float Angle,
			FColor ConeColor = FColor::Orange,
			bool bDrawWireframe = false,
			bool bOnlyLogVisually = true);

		// Logs a line with the visual logger system.
		DbgLogArgs& VisualLogLine(UObject* Owner,
			const FVector& Start,
			const FVector& End,
			float Thickness = 1.f,
			FColor LineColor = FColor::Orange,
			bool bOnlyLogVisually = true);

		// Logs an arrow with the visual logger system.
		DbgLogArgs& VisualLogArrow(UObject* Owner,
			const FVector& Start,
			const FVector& End,
			FColor ArrowColor = FColor::Orange,
			bool bOnlyLogVisually = true);

		// Logs a disk with the visual logger system.
		DbgLogArgs& VisualLogDisk(UObject* Owner,
			const FVector& Start,
			const FVector& UpDir,
			float Radius,
			FColor DiskColor = FColor::Orange,
			uint16 Thickness = 1,
			bool bOnlyLogVisually = true);

		// Logs a capsule with the visual logger system.
		DbgLogArgs& VisualLogCapsule(UObject* Owner,
			const FVector& Base,
			const FRotator Rotation,
			float Radius,
			float HalfHeight,
			FColor CapsuleColor = FColor::Orange,
			bool bDrawWireframe = false,
			bool bOnlyLogVisually = true);

	private:
		static inline FName DefaultLogCategoryName = FName{"Log"};
		
		UWorld* WCOResultValue						= nullptr;
		UObject* VisualLoggerOwnerValue				= nullptr;
		TStringView<TCHAR> DateTimeFormat			= nullptr;
		TFunction<void(EAppReturnType::Type Response)> AppMessageResponse = nullptr;
		
		FString PrefixValue							= {};
		FName CategoryNameValue						= DefaultLogCategoryName;
		FColor ScreenColorValue						= FColor::Transparent;
		
		float ScreenDurationValue					= -1.f;
		TOptional<int32> ScreenKeyValue				= NullOpt;

		EDbgLogOutput OutputDestinationValue		= EDbgLogOutput::Con;
		

		// All the visual logger related variables.
		EDbgVisualLogShape VisualLogShapeValue		= EDbgVisualLogShape::None;
		FVector VisualLogShapeLocationValue			= FVector::ZeroVector;
		FRotator VisualLogShapeRotationValue		= FRotator::ZeroRotator;
		FVector VisualLogShapeScaleValue			= FVector::OneVector;

		// Reuse these for context dependent shapes.
		// For example a sphere just uses VectorOne.X for radius but a
		// Box needs a min and max extent.
		FVector VisualLogVectorOne 					= FVector::ZeroVector;
		FVector VisualLogVectorTwo 					= FVector::ZeroVector;

		FColor VisualLogShapeColorValue				= FColor::Orange;
		
		uint16 bLogConditionValue:1 				= true;
		uint16 bLogSourceLocation:1 				= false;
		uint16 bLogToSlateNotify:1 					= false;
		uint16 bOnlyLogToSlateNotify:1 				= false;
		uint16 bLogToMessageDialog:1 				= false;
		uint16 bOnlyLogToMessageDialog:1 			= false;
		uint16 bLogToEditorMessageLog:1 			= false;
		uint16 bShowEditorMessageLogImmediately:1 	= false;
		uint16 bLogDateAndTime:1 					= false;
		uint16 bDrawWireframeValue:1 				= false;
		uint16 bOnlyUseVisualLogger:1 				= false;

		EAppMsgType::Type AppMsgType				= EAppMsgType::Type::Ok;
		ELogVerbosity::Type VerbosityValue			= ELogVerbosity::Display;
	};



	template<typename... A>
	void Log(int32 UniqueIdentifier, std::source_location Location, const FLogCategoryBase& Category, DbgLogArgs LogArgs, std::wformat_string<TFormatted<A>...> Format, A&&... Args)
	{
		if( LogArgs.VerbosityValue == ELogVerbosity::NoLogging
			|| LogArgs.bLogConditionValue == false )
		{
			return;
		}
		
		/*
		FLogCategory<ELogVerbosity::Log, ELogVerbosity::Log> LogCategory =
			FLogCategory<ELogVerbosity::Type::Log, ELogVerbosity::Type::Log>
				{ FName{ FString::Format( TEXT("dbg{0}"), { LogArgs.CategoryNameValue.ToString()} ) } };
				*/
				

		FLogCategory<ELogVerbosity::Log, ELogVerbosity::Log> LogCategory =
			FLogCategory<ELogVerbosity::Type::Log, ELogVerbosity::Type::Log>(Category.GetCategoryName());
				

		
		// Format the actual log provided from the user.
		FString Message = FormatMessage(std::move(Format), std::forward<A>(Args)...);


		
		// Configure how we present the log now.
		static auto NetModeToStr = [](ENetMode Mode) -> FString
		{
			static FString StandAlone{TEXT("Standalone")};
			static FString DedicatedServer{TEXT("Dedicated Server")};
			static FString ListenServer{TEXT("Listen Server")};
			static FString Client{TEXT("Client")};
			static FString Max{TEXT("MAX")};
			static FString Unknown{TEXT("Unknown")};
			
			switch (Mode)
			{
				case NM_Standalone:			return StandAlone;
				case NM_DedicatedServer:	return DedicatedServer;
				case NM_ListenServer:		return ListenServer;
				case NM_Client:				return Client;
				case NM_MAX:				return Max;
				default:					return Unknown;
			}
		};
		 
		static auto WorldToString = [](UWorld* W)
		{
			FWorldContext* WC = GEngine->GetWorldContextFromWorld(W);
			if (!WC)
			{
				static FString NullWCO = TEXT("NullWorld");
				return NullWCO;
			}
			return FString::Format( TEXT("{0} | Instance: {1}"),
				{ NetModeToStr( W->GetNetMode()), WC->PIEInstance } );
		};

		// make it a little nicer to read.
		static auto SourceLocationToStr = [](std::source_location& L) -> FString
		{
			/*FString FuncName(L.function_name());
			FuncName.ReplaceInline( TEXT(" __cdecl"), TEXT(""));
			return FString::Format( TEXT("File: {0} ({1}), {2} :"),
				{ FPaths::GetCleanFilename( StringCast<wchar_t>(L.file_name()).Get() ), L.line(), FuncName } );
				*/

			FString FuncName(L.function_name());
			FuncName.ReplaceInline( TEXT(" __cdecl"), TEXT(""));
			return FString::Format( TEXT("[{2}] :"),
				{ FuncName } );
		};


		
		if (LogArgs.PrefixValue.IsEmpty() == false)
		{
			if (UWorld* World = LogArgs.WCOResultValue)
			{
				if (LogArgs.bLogSourceLocation)
				{
					Message = FString::Format(TEXT("[{0}] [{1}]: {2} {3}"),
						{ LogArgs.PrefixValue, WorldToString(World), SourceLocationToStr(Location), Message});
				}
				else
				{
					Message = FString::Format(TEXT("[{0}] [{1}]: {2}"),
						{ LogArgs.PrefixValue, WorldToString(World), Message } );
				}
			}
			else
			{
				if (LogArgs.bLogSourceLocation)
				{
					Message = FString::Format( TEXT("[{0}]: {1} {2}"),
						{ LogArgs.PrefixValue, SourceLocationToStr(Location), Message } );
				}
				else
				{
					Message = FString::Format( TEXT("[{0}]: {1}"),
						{ LogArgs.PrefixValue, Message } );
				}
			}
		}
		else if (UWorld* World = LogArgs.WCOResultValue)
		{
			if (LogArgs.bLogSourceLocation)
			{
				Message = FString::Format(TEXT("[{0}]: {1} {2}"),
					{ WorldToString(World), SourceLocationToStr(Location), Message } );
			}
			else
			{
				Message = FString::Format(TEXT("[{0}]: {1}"),
					{ WorldToString(World), Message } );
			}
		}
		else if (LogArgs.bLogSourceLocation)
		{
			Message = FString::Format(TEXT("{0} {1}"), { SourceLocationToStr(Location),  Message } );
		}

		
		if ( LogArgs.bLogDateAndTime )
		{
			if (LogArgs.DateTimeFormat != nullptr)
			{
				Message.InsertAt( 0, FString::Format( TEXT("({0}) "), { FDateTime::Now().ToString( LogArgs.DateTimeFormat.GetData() ) } ) );
			}
			else
			{
				Message.InsertAt( 0, FString::Format( TEXT("({0}) "), { FDateTime::Now().ToString() } ) );
			}
		}

		
		
#if ENABLE_VISUAL_LOG
		// The reason for manually calling these instead of using VLOG is that VLOG wanted to be annoying and
		// assume our log verbosity is a constant IE `ELogVerbosity::MacroVerbosity`
		if( LogArgs.VisualLoggerOwnerValue && FVisualLogger::IsRecording() )
		{
			switch (LogArgs.VisualLogShapeValue)
			{
				case DbgLogArgs::EDbgVisualLogShape::None:
					{
						FVisualLogger::CategorizedLogf( LogArgs.VisualLoggerOwnerValue, LogCategory,
							LogArgs.VerbosityValue, TEXT("%s"), *Message );
						break;
					}
				case DbgLogArgs::EDbgVisualLogShape::Sphere:
					{
#if UE_VERSION_NEWER_THAN(5, 4, 0)
						FVisualLogger::SphereLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							LogArgs.VisualLogShapeLocationValue,
							LogArgs.VisualLogVectorOne.X,
							LogArgs.VisualLogShapeColorValue,
							LogArgs.bDrawWireframeValue,
							TEXT("%s"), *Message );
#else
						FVisualLogger::GeometryShapeLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							LogArgs.VisualLogShapeLocationValue,
							LogArgs.VisualLogVectorOne.X,
							LogArgs.VisualLogShapeColorValue,
							TEXT("%s"), *Message );
#endif
						break;
					}
				case DbgLogArgs::EDbgVisualLogShape::Box:
					{
#if UE_VERSION_NEWER_THAN(5, 4, 0)
						FVisualLogger::BoxLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							FBox{LogArgs.VisualLogVectorOne, LogArgs.VisualLogVectorTwo},
							FMatrix{ FScaleMatrix(LogArgs.VisualLogShapeScaleValue) *
									FRotationMatrix(LogArgs.VisualLogShapeRotationValue) *
									FTranslationMatrix(LogArgs.VisualLogShapeLocationValue) },
							LogArgs.VisualLogShapeColorValue,
							LogArgs.bDrawWireframeValue,
							TEXT("%s"), *Message );
#else
						FVisualLogger::GeometryBoxLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							FBox{LogArgs.VisualLogVectorOne, LogArgs.VisualLogVectorTwo},
							FMatrix{ FScaleMatrix(LogArgs.VisualLogShapeScaleValue) *
									FRotationMatrix(LogArgs.VisualLogShapeRotationValue) *
									FTranslationMatrix(LogArgs.VisualLogShapeLocationValue) },
							LogArgs.VisualLogShapeColorValue,
							TEXT("%s"), *Message );
#endif
						break;
					}
				case DbgLogArgs::EDbgVisualLogShape::Cone:
					{
#if UE_VERSION_NEWER_THAN(5, 4, 0)
						FVisualLogger::ConeLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							LogArgs.VisualLogShapeLocationValue,
							LogArgs.VisualLogVectorOne,
							LogArgs.VisualLogVectorTwo.X,
							LogArgs.VisualLogVectorTwo.Y,
							LogArgs.VisualLogShapeColorValue,
							LogArgs.bDrawWireframeValue,
							TEXT("%s"), *Message );
#else
						FVisualLogger::GeometryShapeLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							LogArgs.VisualLogShapeLocationValue,
							LogArgs.VisualLogVectorOne,
							LogArgs.VisualLogVectorTwo.X,
							LogArgs.VisualLogVectorTwo.Y,
							LogArgs.VisualLogShapeColorValue,
							TEXT("%s"), *Message );
						
						break;
#endif
					}
				case DbgLogArgs::EDbgVisualLogShape::Line:
					{
#if UE_VERSION_NEWER_THAN(5, 4, 0)
						FVisualLogger::SegmentLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							LogArgs.VisualLogShapeLocationValue,
							LogArgs.VisualLogVectorOne,
							LogArgs.VisualLogShapeColorValue,
							static_cast<uint16>(LogArgs.VisualLogVectorTwo.X),
							TEXT("%s"), *Message );
#else
						FVisualLogger::GeometryShapeLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							LogArgs.VisualLogShapeLocationValue,
							LogArgs.VisualLogVectorOne,
							LogArgs.VisualLogShapeColorValue,
							static_cast<uint16>(LogArgs.VisualLogVectorTwo.X),
							TEXT("%s"), *Message );
#endif
	
						break;
					}
				case DbgLogArgs::EDbgVisualLogShape::Arrow:
					{
						FVisualLogger::ArrowLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							LogArgs.VisualLogShapeLocationValue,
							LogArgs.VisualLogVectorOne,
							LogArgs.VisualLogShapeColorValue,
							TEXT("%s"), *Message );
						break;
					}
				case DbgLogArgs::EDbgVisualLogShape::Disk:
					{
						FVisualLogger::CircleLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							LogArgs.VisualLogShapeLocationValue,
							LogArgs.VisualLogVectorOne,
							LogArgs.VisualLogVectorTwo.X,
							LogArgs.VisualLogShapeColorValue,
							static_cast<uint16>(LogArgs.VisualLogVectorTwo.Y),
							TEXT("%s"), *Message );
						break;
					}
				case DbgLogArgs::EDbgVisualLogShape::Capsule:
					{
#if UE_VERSION_NEWER_THAN(5, 4, 0)
						FVisualLogger::CapsuleLogf( LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							LogArgs.VisualLogShapeLocationValue,
							LogArgs.VisualLogVectorOne.X,
							LogArgs.VisualLogVectorOne.Y,
							LogArgs.VisualLogShapeRotationValue.Quaternion(),
							LogArgs.VisualLogShapeColorValue,
							LogArgs.bDrawWireframeValue,
							TEXT("%s"), *Message );
#else
						FVisualLogger::GeometryShapeLogf(LogArgs.VisualLoggerOwnerValue,
							LogCategory, LogArgs.VerbosityValue,
							LogArgs.VisualLogShapeLocationValue,
							LogArgs.VisualLogVectorOne.X,
							LogArgs.VisualLogVectorOne.Y,
							LogArgs.VisualLogShapeRotationValue.Quaternion(),
							LogArgs.VisualLogShapeColorValue,
							TEXT("%s"), *Message );
#endif
						break;
					}
			}
		}
#endif
		if( LogArgs.VisualLoggerOwnerValue && LogArgs.bOnlyUseVisualLogger )
		{
			return;
		}
		
		
		switch (LogArgs.VerbosityValue)
		{
		case ELogVerbosity::Display:
			{
				if( LogArgs.ScreenColorValue == FColor::Transparent )
				{
					LogArgs.ScrnColor( FColor::White );
				}
				
				if( LogArgs.ScreenDurationValue < 0 )
				{
					LogArgs.ScrnDuration(10);
				}
				
				if ( LogArgs.bLogToSlateNotify )
				{
					FNotificationInfo Info{ FText::FromString( Message ) };
					Info.ExpireDuration = 6.f;
					FSlateNotificationManager::Get().AddNotification( Info );
				}

				if ( LogArgs.bLogToEditorMessageLog )
				{
					FMessageLog MsgLog(LogCategory.GetCategoryName());
					MsgLog.Info( FText::FromString( Message ) );

					if ( LogArgs.bShowEditorMessageLogImmediately )
					{
						MsgLog.Open( EMessageSeverity::Type::Info );
					}
				}
				
				break;
			}
		case ELogVerbosity::Warning:
			{
				if( LogArgs.ScreenColorValue == FColor::Transparent )
				{
					LogArgs.ScrnColor( FColor::Yellow );
				}
				
				if( LogArgs.ScreenDurationValue < 0 )
				{
					LogArgs.ScrnDuration( 20 );
				}
				
				if ( LogArgs.bLogToSlateNotify )
				{
					FNotificationInfo Info{ FText::FromString( Message ) };
					Info.ExpireDuration = 15.f;
					FSlateNotificationManager::Get().AddNotification( Info );
				}

				if ( LogArgs.bLogToEditorMessageLog )
				{
					FMessageLog MsgLog(LogCategory.GetCategoryName());
					MsgLog.Warning( FText::FromString( Message ) );
					
					if ( LogArgs.bShowEditorMessageLogImmediately )
					{
						MsgLog.Open( EMessageSeverity::Type::Warning );
					}
				}
				break;
			}
		case ELogVerbosity::Error:
			{
				if( LogArgs.ScreenColorValue == FColor::Transparent )
				{
					LogArgs.ScrnColor( FColor::Red );
				}
				
				if( LogArgs.ScreenDurationValue < 0 )
				{
					LogArgs.ScrnDuration( 30 );
				}
				
				if ( LogArgs.bLogToSlateNotify )
				{
					FNotificationInfo Info{ FText::FromString( Message ) };
					Info.ExpireDuration = 30.f;
					FSlateNotificationManager::Get().AddNotification( Info );
				}

				if ( LogArgs.bLogToEditorMessageLog )
				{
					FMessageLog MsgLog(LogCategory.GetCategoryName());
					MsgLog.Error( FText::FromString( Message ) );
					
					if ( LogArgs.bShowEditorMessageLogImmediately )
					{
						MsgLog.Open( EMessageSeverity::Type::Error );
					}
				}
				
				break;
			}
		case ELogVerbosity::Fatal:
			{
				if( LogArgs.ScreenColorValue == FColor::Transparent )
				{
					LogArgs.ScrnColor( FColor::Blue );
				}
				
				if( LogArgs.ScreenDurationValue < 0 )
				{
					LogArgs.ScrnDuration( 30 );
				}

				if ( LogArgs.bLogToSlateNotify )
				{
					FNotificationInfo Info{ FText::FromString( Message ) };
					Info.ExpireDuration = 30.f;
					FSlateNotificationManager::Get().AddNotification( Info );
				}

				if ( LogArgs.bLogToEditorMessageLog )
				{
					FMessageLog MsgLog(LogCategory.GetCategoryName());
					MsgLog.Error( FText::FromString( Message ) );
					
					if ( LogArgs.bShowEditorMessageLogImmediately )
					{
						MsgLog.Open( EMessageSeverity::Type::Error );
					}
				}
				
				break;
			}
		default:
		case ELogVerbosity::Verbose:
			{
				if( LogArgs.ScreenColorValue == FColor::Transparent )
				{
					LogArgs.ScrnColor( FColor::White );
				}
				
				if( LogArgs.ScreenDurationValue < 0 )
				{
					LogArgs.ScrnDuration( 10 );
				}

				if ( LogArgs.bLogToSlateNotify )
				{
					FNotificationInfo Info{ FText::FromString( Message ) };
					Info.ExpireDuration = 6.f;
					FSlateNotificationManager::Get().AddNotification( Info );
				}

				if ( LogArgs.bLogToEditorMessageLog )
				{
					FMessageLog MsgLog(LogCategory.GetCategoryName());
					MsgLog.Info( FText::FromString( Message ) );
					
					if ( LogArgs.bShowEditorMessageLogImmediately )
					{
						MsgLog.Open( EMessageSeverity::Type::Info );
					}
				}
				
				break;
			}
		}


		if ( LogArgs.bLogToMessageDialog )
		{
			EAppReturnType::Type Response = FMessageDialog::Open( LogArgs.AppMsgType,
				FText::FromString( Message ), FText::FromName( LogCategory.GetCategoryName() ) );

			if ( LogArgs.AppMessageResponse )
			{
				LogArgs.AppMessageResponse( Response );
			}
		}

		// Return early if we had no intention of logging to the screen/console
		if (	LogArgs.bOnlyLogToSlateNotify
			|| 	LogArgs.bOnlyLogToMessageDialog
			|| (LogArgs.bLogToEditorMessageLog && LogArgs.OutputDestinationValue == EDbgLogOutput::Con) ) // The output message log already handles console logging for us.
		{
			return;
		}


		uint64 Key = 0;
		if ( LogArgs.OutputDestinationValue != EDbgLogOutput::Con )
		{
			// Make sure we never have a key clash
			static std::hash<std::string> Hasher;
			int PIEID = 0;
#if UE_VERSION_OLDER_THAN(5, 5, 0)
			PIEID = GPlayInEditorID;
#else
			PIEID = UE::GetPlayInEditorID();
#endif
			
			Key = Location.line() + Hasher( Location.function_name()) + PIEID +
				UniqueIdentifier + ( LogArgs.ScreenKeyValue.IsSet() ? LogArgs.ScreenKeyValue.GetValue() : 0 );
		}
		

		switch (LogArgs.OutputDestinationValue)
		{
		case EDbgLogOutput::Con:
			{
				switch (LogArgs.VerbosityValue)
				{
					case ELogVerbosity::Display: 	UE_LOG(LogCategory, Display, TEXT("%s"), *Message); break;
					case ELogVerbosity::Warning: 	UE_LOG(LogCategory, Warning, TEXT("%s"), *Message); break;
					case ELogVerbosity::Error:		UE_LOG(LogCategory, Error, 	 TEXT("%s"), *Message); break;
					case ELogVerbosity::Fatal:		UE_LOG(LogCategory, Fatal, 	 TEXT("%s"), *Message); break;
					case ELogVerbosity::Verbose:	UE_LOG(LogCategory, Verbose, TEXT("%s"), *Message); break;
					default:						UE_LOG(LogCategory, Display, TEXT("%s"), *Message); break;
				}
			}
			break;
		case EDbgLogOutput::Scr:
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage( Key, LogArgs.ScreenDurationValue,
						LogArgs.ScreenColorValue, Message, true );
				}
				break;
			}
		case EDbgLogOutput::Both:
			{
				// Only output log if we arent already writing it to the msg log since that handles console outputting.
				if (LogArgs.bLogToEditorMessageLog == false)
				{
					switch (LogArgs.VerbosityValue)
					{
						case ELogVerbosity::Display:	UE_LOG(LogCategory, Display, TEXT("%s"), *Message); break;
						case ELogVerbosity::Warning:	UE_LOG(LogCategory, Warning, TEXT("%s"), *Message); break;
						case ELogVerbosity::Error:		UE_LOG(LogCategory, Error,   TEXT("%s"), *Message); break;
						case ELogVerbosity::Fatal:		UE_LOG(LogCategory, Fatal,   TEXT("%s"), *Message); break;
						case ELogVerbosity::Verbose:	UE_LOG(LogCategory, Verbose, TEXT("%s"), *Message); break;
						default:						UE_LOG(LogCategory, Display, TEXT("%s"), *Message); break;
					}
				}
				
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(Key, LogArgs.ScreenDurationValue,
						LogArgs.ScreenColorValue, Message, true );
				}
				break;
			}
		}
	}




	

	inline DbgLogArgs::ThisClass& DbgLogArgs::DrawDebugCapsule(const UObject* WorldContextObject,
		const FVector& Center, float HalfHeight, float Radius,
		const FQuat& Rotation, const FColor& Color, bool bPersistentLines,
		float LifeTime, uint8 DepthPriority, float Thickness)
	{
		if (const UWorld* W = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::ReturnNull ))
		{
			::DrawDebugCapsule( W, Center, HalfHeight, Radius,
			                    Rotation, Color, bPersistentLines, LifeTime,
			                    DepthPriority, Thickness );
		}
			
		return *this;
	}

	
	inline DbgLogArgs::ThisClass& DbgLogArgs::DrawDebugCone(const UObject* WorldContextObject,
		const FVector& Origin, const FVector& Direction, float Length,
		float AngleWidth, float AngleHeight, int32 NumSides,
		const FColor& Color, bool bPersistentLines, float LifeTime,
		uint8 DepthPriority, float Thickness)
	{
		if (const UWorld* W = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::ReturnNull ))
		{
			::DrawDebugCone( W, Origin, Direction, Length,
			                 FMath::DegreesToRadians(AngleWidth), FMath::DegreesToRadians(AngleHeight), NumSides, Color,
			                 bPersistentLines, LifeTime, DepthPriority, Thickness );
				
		}
		return *this;
	}

	
	inline DbgLogArgs::ThisClass& DbgLogArgs::DrawDebugCylinder(const UObject* WorldContextObject,
		const FVector& Start, const FVector& End, float Radius,
		int32 Segments, const FColor& Color, bool bPersistentLines,
		float LifeTime, uint8 DepthPriority, float Thickness)
	{
		if (const UWorld* W = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::ReturnNull ))
		{
			::DrawDebugCylinder( W, Start, End, Radius,
			                     Segments, Color, bPersistentLines, LifeTime,
			                     DepthPriority, Thickness );
		}
		
			
		return *this;
	}

	
	inline DbgLogArgs::ThisClass& DbgLogArgs::DrawDebugDirectionalArrow(const UObject* WorldContextObject,
		const FVector& LineStart, const FVector& LineEnd, float ArrowSize, const FColor& Color,
		bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
	{
		if (const UWorld* W = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::ReturnNull ))
		{
			::DrawDebugDirectionalArrow( W, LineStart, LineEnd, ArrowSize,
			                             Color, bPersistentLines, LifeTime, DepthPriority,
			                             Thickness );
		}
		return *this;
	}

	
	inline DbgLogArgs::ThisClass& DbgLogArgs::DrawDebugLine(const UObject* WorldContextObject,
		const FVector& LineStart, const FVector& LineEnd, const FColor& Color,
		bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
	{
		if (const UWorld* W = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::ReturnNull ))
		{
			::DrawDebugLine( W, LineStart, LineEnd, Color,
			                 bPersistentLines, LifeTime, DepthPriority,
			                 Thickness );
		}
		return *this;
	}

	
	inline DbgLogArgs::ThisClass& DbgLogArgs::DrawDebugPoint(const UObject* WorldContextObject,
		const FVector& Position, float Size, const FColor& Color,
		bool bPersistentLines, float LifeTime, uint8 DepthPriority)
	{
		if (const UWorld* W = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::ReturnNull ))
		{
			::DrawDebugPoint( W, Position, Size, Color,
			                  bPersistentLines, LifeTime, DepthPriority );
		}
		return *this;
	}

	
	inline DbgLogArgs::ThisClass& DbgLogArgs::DrawDebugSphere(const UObject* WorldContextObject,
		const FVector& Center, float Radius, int32 Segments, const FColor& Color,
		bool bPersistentLines, float LifeTime, uint8 DepthPriority,
		float Thickness)
	{
		if (const UWorld* W = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::ReturnNull ))
		{
			::DrawDebugSphere( W, Center, Radius, Segments,
			                   Color, bPersistentLines, LifeTime, DepthPriority,
			                   Thickness );
		}
		return *this;
	}
	

	inline DbgLogArgs::ThisClass& DbgLogArgs::DrawDebugString(const UObject* WorldContextObject,
		const FVector& TextLocation, const FString& Text,
		AActor* TestBaseActor, const FColor& TextColor, float Duration)
	{
		if (const UWorld* W = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::ReturnNull ))
		{
			::DrawDebugString( W, TextLocation, Text, TestBaseActor,
			                   TextColor, Duration );
		}
		return *this;
	}

	inline DbgLogArgs::ThisClass& DbgLogArgs::DrawDebugBox(const UObject* WorldContextObject,
		const FVector& Center, const FVector& Extent, const FColor& Color,
		bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
	{
		if (const UWorld* W = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::ReturnNull ))
		{
			::DrawDebugBox( W, Center, Extent, Color,
			                bPersistentLines, LifeTime, DepthPriority, Thickness );
			
		}
		return *this;
	}

	
	inline DbgLogArgs& DbgLogArgs::VisualLogText(UObject* Owner, bool bOnlyLogVisually)
	{
		VisualLoggerOwnerValue = Owner;
		VisualLogShapeValue = EDbgVisualLogShape::None;
		bOnlyUseVisualLogger = bOnlyLogVisually;
		return *this;
	}

	
	inline DbgLogArgs& DbgLogArgs::VisualLogSphere(UObject* Owner, const FVector& Location, float Radius,
		bool bDrawWireframe, bool bOnlyLogVisually)
	{
		// Only support a single visual log per macro.
		if (VisualLoggerOwnerValue == nullptr)
		{
			VisualLoggerOwnerValue = Owner;
			VisualLogShapeValue = EDbgVisualLogShape::Sphere;

			VisualLogShapeLocationValue = Location;
			VisualLogVectorOne.X = Radius;

			bDrawWireframeValue = bDrawWireframe;
			bOnlyUseVisualLogger = bOnlyLogVisually;
		}
		return *this;
	}

	
	inline DbgLogArgs& DbgLogArgs::VisualLogBox(UObject* Owner, const FVector& MinExtent,
		const FVector& MaxExtent, const FVector& Location, const FRotator& Rotation,
		FColor BoxColor, bool bDrawWireframe, bool bOnlyLogVisually)
	{
		// Only support a single visual log per macro.
		if (VisualLoggerOwnerValue == nullptr)
		{
			VisualLoggerOwnerValue = Owner;
			VisualLogShapeValue = EDbgVisualLogShape::Box;
			VisualLogShapeColorValue = BoxColor;

			VisualLogShapeLocationValue = Location;
			VisualLogShapeRotationValue = Rotation;

			VisualLogVectorOne = MinExtent;
			VisualLogVectorTwo = MaxExtent;
				
			bDrawWireframeValue = bDrawWireframe;
			bOnlyUseVisualLogger = bOnlyLogVisually;
		}
		return *this;
	}

	
	inline DbgLogArgs& DbgLogArgs::VisualLogBounds(UObject* Owner,
		const AActor* ActorToGetBoundsFrom, FColor BoundsColor,
		bool bDrawWireframe, bool bOnlyLogVisually)
	{
		// Only support a single visual log per macro.
		if (VisualLoggerOwnerValue == nullptr && ::IsValid(ActorToGetBoundsFrom))
		{
			FBox B = ActorToGetBoundsFrom->CalculateComponentsBoundingBoxInLocalSpace(); 
			VisualLogBox(Owner, B.Min, B.Max, ActorToGetBoundsFrom->GetActorLocation(), 
				ActorToGetBoundsFrom->GetActorRotation(),
				BoundsColor, bDrawWireframe, bOnlyLogVisually);
		}
		return *this;
	}

	
	inline DbgLogArgs& DbgLogArgs::VisualLogCone(UObject* Owner,
		const FVector& Location, const FVector& Direction,
	    float Length, float Angle, FColor ConeColor, bool bDrawWireframe, bool bOnlyLogVisually)
	{
		// Only support a single visual log per macro.
		if (VisualLoggerOwnerValue == nullptr)
		{
			VisualLoggerOwnerValue = Owner;
			VisualLogShapeValue = EDbgVisualLogShape::Cone;
			VisualLogShapeColorValue = ConeColor;

			VisualLogShapeLocationValue = Location;

			VisualLogVectorOne = Direction;
			VisualLogVectorTwo.X = Length;
			VisualLogVectorTwo.Y = Angle;
				
			bDrawWireframeValue = bDrawWireframe;
			bOnlyUseVisualLogger = bOnlyLogVisually;
		}
		return *this;
	}

	
	inline DbgLogArgs& DbgLogArgs::VisualLogLine(UObject* Owner,
		const FVector& Start, const FVector& End,
		float Thickness, FColor LineColor,bool bOnlyLogVisually)
	{
		// Only support a single visual log per macro.
		if (VisualLoggerOwnerValue == nullptr)
		{
			VisualLoggerOwnerValue = Owner;
			VisualLogShapeValue = EDbgVisualLogShape::Line;
			VisualLogShapeColorValue = LineColor;

			VisualLogShapeLocationValue = Start;
			VisualLogVectorOne = End;
			VisualLogVectorTwo.X = Thickness;
				
			bOnlyUseVisualLogger = bOnlyLogVisually;
		}
		return *this;
	}

	
	inline DbgLogArgs& DbgLogArgs::VisualLogArrow(UObject* Owner,
		const FVector& Start, const FVector& End,
		FColor ArrowColor,bool bOnlyLogVisually)
	{
		// Only support a single visual log per macro.
		if (VisualLoggerOwnerValue == nullptr)
		{
			VisualLoggerOwnerValue = Owner;
			VisualLogShapeValue = EDbgVisualLogShape::Arrow;
			VisualLogShapeColorValue = ArrowColor;

			VisualLogShapeLocationValue = Start;
			VisualLogVectorOne = End;
				
			bOnlyUseVisualLogger = bOnlyLogVisually;
		}
		return *this;
	}

	
	inline DbgLogArgs& DbgLogArgs::VisualLogDisk(UObject* Owner,
		const FVector& Start, const FVector& UpDir, float Radius,
		FColor ArrowColor, uint16 Thickness, bool bOnlyLogVisually)
	{
		// Only support a single visual log per macro.
		if (VisualLoggerOwnerValue == nullptr)
		{
			VisualLoggerOwnerValue = Owner;
			VisualLogShapeValue = EDbgVisualLogShape::Disk;
			VisualLogShapeColorValue = ArrowColor;

			VisualLogShapeLocationValue = Start;
			VisualLogVectorOne = UpDir;
			VisualLogVectorTwo.X = Radius;
			VisualLogVectorTwo.Y = Thickness;
				
			bOnlyUseVisualLogger = bOnlyLogVisually;
		}
		return *this;
	}

	
	inline DbgLogArgs& DbgLogArgs::VisualLogCapsule(UObject* Owner,
		const FVector& Base, const FRotator Rotation, float Radius,
		float HalfHeight, FColor CapsuleColor, bool bDrawWireframe, bool bOnlyLogVisually)
	{
		// Only support a single visual log per macro.
		if (VisualLoggerOwnerValue == nullptr)
		{
			VisualLoggerOwnerValue = Owner;
			VisualLogShapeValue = EDbgVisualLogShape::Capsule;
			VisualLogShapeColorValue = CapsuleColor;

			VisualLogShapeLocationValue = Base;
			VisualLogShapeRotationValue = Rotation;
			VisualLogVectorOne.X = HalfHeight;
			VisualLogVectorOne.Y = Radius;
				
			bDrawWireframeValue = bDrawWireframe;
			bOnlyUseVisualLogger = bOnlyLogVisually;
		}
		return *this;
	}
}



#else
	#define _INTERNAL_DBGLOGV(Args, Msg, Name, ...) 
	#define dbgLOG(Msg, ...)
	#define dbgLOGV(Args, Msg, ...)

#endif
