// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "FUOrientedBox.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FUUtilityLibrary.generated.h"


/**
 * 
 */
UCLASS(DisplayName = "Fishy Utility Library")
class FISHYUTILS_API UFUUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta=(DisplayName = "IntVector2 + IntVector2", CompactNodeTitle = "+", ScriptMethod = "Add", ScriptOperator = "+;+=", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category="Math|IntVector2")
	static FIntVector2 Add_IntVector2IntVector2(FIntVector2 A, FIntVector2 B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "IntVector2 - IntVector2", CompactNodeTitle = "-", ScriptMethod = "Subtract", ScriptOperator = "-;-=", Keywords = "- subtract minus", CommutativeAssociativeBinaryOperator = "true"), Category="Math|IntVector2")
	static FIntVector2 Substract_IntVector2IntVector2(FIntVector2 A, FIntVector2 B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "IntVector2 * IntVector2", CompactNodeTitle = "*", ScriptMethod = "Multiply", ScriptOperator = "*;*=", Keywords = "+ multiply", CommutativeAssociativeBinaryOperator = "true"), Category="Math|IntVector2")
	static FIntVector2 Multiply_IntVector2IntVector2(FIntVector2 A, FIntVector2 B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "IntVector2 / IntVector2", CompactNodeTitle = "/", ScriptMethod = "Divide", ScriptOperator = "/;/=", Keywords = "/ divide division", CommutativeAssociativeBinaryOperator = "true"), Category="Math|IntVector2")
	static FIntVector2 Divide_IntVector2IntVector2(FIntVector2 A, FIntVector2 B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Equal (IntVector2)", CompactNodeTitle = "==", Keywords = "== equal"), Category="Math|IntVector2")
	static bool Equal_IntVector2IntVector2(FIntVector2 A, FIntVector2 B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Not Equal (IntVector2)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category="Math|IntVector2")
	static bool NotEqual_IntVector2IntVector2(FIntVector2 A, FIntVector2 B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "IntVector + IntVector", CompactNodeTitle = "+", ScriptMethod = "Add", ScriptOperator = "+;+=", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category="Math|IntVector")
	static FIntVector Add_IntVectorIntVector(FIntVector A, FIntVector B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "IntVector - IntVector", CompactNodeTitle = "-", ScriptMethod = "Subtract", ScriptOperator = "-;-=", Keywords = "- subtract minus", CommutativeAssociativeBinaryOperator = "true"), Category="Math|IntVector")
	static FIntVector Substract_IntVectorIntVector(FIntVector A, FIntVector B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "IntVector * IntVector", CompactNodeTitle = "*", ScriptMethod = "Multiply", ScriptOperator = "*;*=", Keywords = "+ multiply", CommutativeAssociativeBinaryOperator = "true"), Category="Math|IntVector")
	static FIntVector Multiply_IntVectorIntVector(FIntVector A, FIntVector B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "IntVector / IntVector", CompactNodeTitle = "/", ScriptMethod = "Divide", ScriptOperator = "/;/=", Keywords = "/ divide division", CommutativeAssociativeBinaryOperator = "true"), Category="Math|IntVector")
	static FIntVector Divide_IntVectorIntVector(FIntVector A, FIntVector B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Equal (IntVector)", CompactNodeTitle = "==", Keywords = "== equal"), Category="Math|IntVector")
	static bool Equal_IntVectorIntVector(FIntVector A, FIntVector B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Not Equal (IntVector)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category="Math|IntVector")
	static bool NotEqual_IntVectorIntVector(FIntVector A, FIntVector B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "rotator + rotator", CompactNodeTitle = "+", ScriptMethod = "Add", ScriptOperator = "+;+=", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category="Math|Rotator")
	static FRotator Add_RotatorRotator(FRotator A, FRotator B);
	
	UFUNCTION(BlueprintPure, meta=(DisplayName = "rotator - rotator", CompactNodeTitle = "-", ScriptMethod = "Subtract", ScriptOperator = "-;-=", Keywords = "- subtract minus"), Category="Math|Rotator")
	static FRotator Subtract_RotatorRotator(FRotator A, FRotator B);
	
	UFUNCTION(BlueprintPure, meta=(Keywords="+1, Increment", CompactNodeTitle="+1"))
	static int32 IntPlusOne(int32 X) { return X + 1; }
	
	UFUNCTION(BlueprintPure, meta=(Keywords="-1, Decrement", CompactNodeTitle="-1"))
	static int32 IntMinusOne(int32 X) { return X - 1; }
	
	UFUNCTION(BlueprintPure, meta=(Keywords="*-1, Invert", CompactNodeTitle="*-1"))
	static float Invert(float X) { return X * -1; }
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility|Generic", meta=(WorldContext="WorldContextObject"))
	static bool AsyncOpenLevel(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> Level, bool bAbsolute = true, FString Options = "");
	
	/**
	 * Find the smallest angle between two headings (in degrees).
	 * Return delta in [-180,180] range.
	 */
	UFUNCTION(BlueprintPure, Category="FishyUtils|Utility|Math")
	static float FindDeltaAngleDegrees(float A, float B);
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility")
	static void StringArrayToString(const TArray<FString>& Array, FString& OutString, const FString& Separator = ", ", const FString& Prefix = "[", const FString& Suffix = "]");
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility")
	static void IntVectorArrayToString(const TArray<FIntVector>& Array, FString& OutString, const FString& Separator = ", ", const FString& Prefix = "[", const FString& Suffix = "]");
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility")
	static void IntVector2ArrayToString(const TArray<FIntVector2>& Array, FString& OutString, const FString& Separator = ", ", const FString& Prefix = "[", const FString& Suffix = "]");
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility")
	static void VectorArrayToString(const TArray<FVector>& Array, FString& OutString, const FString& Separator = ", ", const FString& Prefix = "[", const FString& Suffix = "]");
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility")
	static void RotatorArrayToString(const TArray<FRotator>& Array, FString& OutString, const FString& Separator = ", ", const FString& Prefix = "[", const FString& Suffix = "]");
	
	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility")
	static void TransformArrayToString(const TArray<FTransform>& Array, FString& OutString, const FString& Separator = ", ", const FString& Prefix = "[", const FString& Suffix = "]");
};
