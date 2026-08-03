// By hzFishy - 2026 - Do whatever you want with it.

#pragma once


class UCapsuleComponent;


namespace FU::VisualLogger
{
	inline FVector GetTextLocOffset(float ZOffset) { return FVector(0, 0, ZOffset); }
	inline FVector GetTextLocOffset(double ZOffset) { return FVector(0, 0, ZOffset); }
	inline FVector GetTextLocOffset(int32 ZOffset) { return FVector(0, 0, ZOffset); }
	inline FVector GetTextLocOffset(FVector Offset) { return Offset; }
	
	FISHYUTILS_API float GetCapsuleHalfHeight(const UCapsuleComponent* Capsule);
	FISHYUTILS_API float GetCapsuleRadius(const UCapsuleComponent* Capsule);
	FISHYUTILS_API FVector GetCapsuleLocation(const UCapsuleComponent* Capsule);
	FISHYUTILS_API FQuat GetCapsuleRotation(const UCapsuleComponent* Capsule);
}

	
	/*----------------------------------------------------------------------------
		Text
	----------------------------------------------------------------------------*/
#define FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, TextLocation, Color, Format, ...) \
	UE_VLOG_SPHERE(LogOwner, CategoryName, Verbosity, TextLocation, 0, Color, Format, ##__VA_ARGS__); \

	
	/*----------------------------------------------------------------------------
		ARROWS
	----------------------------------------------------------------------------*/
/** Draw arrow using ScaledDirection. With no text */
#define FU_VLOG_ARROW_DIR(LogOwner, CategoryName, Verbosity, SegmentStart, ScaledDirection, Color) \
	UE_VLOG_ARROW(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentStart + ScaledDirection, Color, TEXT_EMPTY); \
	
/** Draw arrow using ScaledDirection. With text draw at the start */
#define FU_VLOG_ARROW_DIR_TEXT_START(LogOwner, CategoryName, Verbosity, SegmentStart, ScaledDirection, Color, TextLocationOffset, Format, ...) \
	UE_VLOG_ARROW(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentStart + ScaledDirection, Color, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, SegmentStart + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);
	
/** Draw arrow using ScaledDirection. With text draw at the middle */
#define FU_VLOG_ARROW_DIR_TEXT_MID(LogOwner, CategoryName, Verbosity, SegmentStart, ScaledDirection, Color, TextLocationOffset, Format, ...) \
	UE_VLOG_ARROW(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentStart + ScaledDirection, Color, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, (SegmentStart + SegmentStart + ScaledDirection) / 2 + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);
	
/** Draw arrow using ScaledDirection. With text draw at the end*/
#define FU_VLOG_ARROW_DIR_TEXT_END(LogOwner, CategoryName, Verbosity, SegmentStart, ScaledDirection, Color, TextLocationOffset, Format, ...) \
	UE_VLOG_ARROW(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentStart + ScaledDirection, Color, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, SegmentStart + ScaledDirection + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);


/** Draw arrow using default SegmentEnd. With no text */
#define FU_VLOG_ARROW(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color) \
	UE_VLOG_ARROW(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, TEXT_EMPTY); \

/** Draw arrow using default SegmentEnd. With text draw at the start */
#define FU_VLOG_ARROW_TEXT_START(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, TextLocationOffset, Format, ...) \
	UE_VLOG_ARROW(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, SegmentStart + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);


/** Draw arrow using default SegmentEnd. With text draw at the middle */
#define FU_VLOG_ARROW_TEXT_MID(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, TextLocationOffset, Format, ...) \
	UE_VLOG_ARROW(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, (SegmentStart + SegmentEnd) / 2 + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);


/** Draw arrow using default SegmentEnd. With text draw at the end */
#define FU_VLOG_ARROW_TEXT_END(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, TextLocationOffset, Format, ...) \
	UE_VLOG_ARROW(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, SegmentEnd + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);

	
	/*----------------------------------------------------------------------------
		SEGMENTS
	----------------------------------------------------------------------------*/
/** Draw a segment with no text */
#define FU_VLOG_SEGMENT(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, Thickness) \
	UE_VLOG_SEGMENT_THICK(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, Thickness, TEXT_EMPTY); \

/** Draw a segment with text at the start */
#define FU_VLOG_SEGMENT_TEXT_START(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, Thickness, TextLocationOffset, Format, ...) \
	UE_VLOG_SEGMENT_THICK(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, Thickness, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, SegmentStart + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);

/** Draw a segment with text at the middle */
#define FU_VLOG_SEGMENT_TEXT_MID(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, Thickness, TextLocationOffset, Format, ...) \
	UE_VLOG_SEGMENT_THICK(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, Thickness, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, (SegmentStart + SegmentEnd) / 2 + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);

/** Draw a segment with text at the end */
#define FU_VLOG_SEGMENT_TEXT_END(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, Thickness, TextLocationOffset, Format, ...) \
	UE_VLOG_SEGMENT_THICK(LogOwner, CategoryName, Verbosity, SegmentStart, SegmentEnd, Color, Thickness, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, SegmentEnd + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);

	
	/*----------------------------------------------------------------------------
		SPHERES
	----------------------------------------------------------------------------*/
/** Draw a sphere with no text */
#define FU_VLOG_SPHERE(LogOwner, CategoryName, Verbosity, Location, Radius, Color) \
	UE_VLOG_SPHERE(LogOwner, CategoryName, Verbosity, Location, Radius, Color, TEXT_EMPTY); \

/** Draw a sphere with text */
#define FU_VLOG_SPHERE_TEXT(LogOwner, CategoryName, Verbosity, Location, Radius, Color, TextLocationOffset, Format, ...) \
	UE_VLOG_SPHERE(LogOwner, CategoryName, Verbosity, Location, Radius, Color, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, Location + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);

/** Draw a wired sphere with no text */
#define FU_VLOG_SPHEREWIRED(LogOwner, CategoryName, Verbosity, Location, Radius, Color) \
	UE_VLOG_WIRESPHERE(LogOwner, CategoryName, Verbosity, Location, Radius, Color, TEXT_EMPTY); \

/** Draw a wired sphere with text */
#define FU_VLOG_SPHEREWIRED_TEXT(LogOwner, CategoryName, Verbosity, Location, Radius, Color, TextLocationOffset, Format, ...) \
	UE_VLOG_WIRESPHERE(LogOwner, CategoryName, Verbosity, Location, Radius, Color, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, Location + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__);

	
	/*----------------------------------------------------------------------------
		CAPSULES
	----------------------------------------------------------------------------*/
/** Draw capsule with no text */
#define FU_VLOG_CAPSULE(LogOwner, CategoryName, Verbosity, Center, HalfHeight, Radius, Rotation, Color) \
	UE_VLOG_CAPSULE(LogOwner, CategoryName, Verbosity, Center - (HalfHeight * Rotation.GetUpVector()), HalfHeight, Radius, Rotation, Color, TEXT_EMPTY); \


/** Draw capsule with no text using UCapsuleComponent */
#define UE_VLOG_CAPSULE_WITHCOMPONENT(LogOwner, CategoryName, Verbosity, CapsuleComponent, Color) \
	{ \
		const FVector Center = FU::VisualLogger::GetCapsuleLocation(CapsuleComponent); \
		const float HalfHeight = FU::VisualLogger::GetCapsuleHalfHeight(CapsuleComponent); \
		const float Radius = FU::VisualLogger::GetCapsuleRadius(CapsuleComponent); \
		const FQuat Rotation = FU::VisualLogger::GetCapsuleRotation(CapsuleComponent); \
		FU_VLOG_CAPSULE(LogOwner, CategoryName, Verbosity, Center, HalfHeight, Radius, Rotation, Color); \
	} \

/** Draw capsule with no text using UCapsuleComponent but with override location */
#define UE_VLOG_CAPSULE_WITHCOMPONENT_LOCATIONOVERRIDE(LogOwner, CategoryName, Verbosity, CapsuleComponent, CenterLocation, Color) \
	{ \
		const FVector Center = CenterLocation; \
		const float HalfHeight = FU::VisualLogger::GetCapsuleHalfHeight(CapsuleComponent); \
		const float Radius = FU::VisualLogger::GetCapsuleRadius(CapsuleComponent); \
		const FQuat Rotation = FU::VisualLogger::GetCapsuleRotation(CapsuleComponent); \
		FU_VLOG_CAPSULE(LogOwner, CategoryName, Verbosity, Center, HalfHeight, Radius, Rotation, Color); \
	} \

/** Draw wired capsule with no text */
#define FU_VLOG_CAPSULEWIRED(LogOwner, CategoryName, Verbosity, Center, HalfHeight, Radius, Rotation, Color) \
	UE_VLOG_WIRECAPSULE(LogOwner, CategoryName, Verbosity, Center - (HalfHeight * Rotation.GetUpVector()), HalfHeight, Radius, Rotation, Color, TEXT_EMPTY); \

/** Draw wired capsule with no text from UCapsuleComponent */
#define FU_VLOG_CAPSULEWIRED_WITHCOMPONENT(LogOwner, CategoryName, Verbosity, CapsuleComponent, Color) \
	{ \
		const FVector Center = FU::VisualLogger::GetCapsuleLocation(CapsuleComponent); \
		const float HalfHeight = FU::VisualLogger::GetCapsuleHalfHeight(CapsuleComponent); \
		const float Radius = FU::VisualLogger::GetCapsuleRadius(CapsuleComponent); \
		const FQuat Rotation = FU::VisualLogger::GetCapsuleRotation(CapsuleComponent); \
		FU_VLOG_CAPSULEWIRED(LogOwner, CategoryName, Verbosity, Center, HalfHeight, Radius, Rotation, Color); \
	} \

/** Draw wired capsule with no text from UCapsuleComponent with override location*/
#define FU_VLOG_CAPSULEWIRED_WITHCOMPONENT_LOCATIONOVERRIDE(LogOwner, CategoryName, Verbosity, CapsuleComponent, CenterLocation, Color) \
	{ \
		const FVector Center = CenterLocation; \
		const float HalfHeight = FU::VisualLogger::GetCapsuleHalfHeight(CapsuleComponent); \
		const float Radius = FU::VisualLogger::GetCapsuleRadius(CapsuleComponent); \
		const FQuat Rotation = FU::VisualLogger::GetCapsuleRotation(CapsuleComponent); \
		FU_VLOG_CAPSULEWIRED(LogOwner, CategoryName, Verbosity, Center, HalfHeight, Radius, Rotation, Color); \
	} \



/* 
 * TODO: next targets
 * 
 * 
#define UE_VLOG_BOX(LogOwner, CategoryName, Verbosity, Box, Color, Format, ...)
#define UE_VLOG_WIREBOX(LogOwner, CategoryName, Verbosity, Box, Color, Format, ...)

#define UE_VLOG_OBOX(LogOwner, CategoryName, Verbosity, Box, Matrix, Color, Format, ...)
#define UE_VLOG_WIREOBOX(LogOwner, CategoryName, Verbosity, Box, Matrix, Color, Format, ...)
*/



/** 
 *  Draw a wired sphere of the given radius and a smaller sphere acting like the center of it.
 *  No text.
 */
#define FU_VLOG_CONCAT_SPHEREWIRED_WITHCENTER(LogOwner, CategoryName, Verbosity, Location, Radius, Color) \
	UE_VLOG_WIRESPHERE(LogOwner, CategoryName, Verbosity, Location, Radius, Color, TEXT_EMPTY); \
	UE_VLOG_SPHERE(LogOwner, CategoryName, Verbosity, Location, 5, Color, TEXT_EMPTY); \
	
/** 
*  Draw a wired sphere of the given radius and text and a smaller sphere acting like the center of it.
*  With text.
*/
#define FU_VLOG_CONCAT_SPHEREWIRED_WITHCENTER_TEXT(LogOwner, CategoryName, Verbosity, Location, Radius, Color, TextLocationOffset, Format, ...) \
	UE_VLOG_WIRESPHERE(LogOwner, CategoryName, Verbosity, Location, Radius, Color, TEXT_EMPTY); \
	UE_VLOG_SPHERE(LogOwner, CategoryName, Verbosity, Location, 5, Color, TEXT_EMPTY); \
	FU_VLOG_TEXT(LogOwner, CategoryName, Verbosity, Location + FU::VisualLogger::GetTextLocOffset(TextLocationOffset), Color, Format, ##__VA_ARGS__); \
