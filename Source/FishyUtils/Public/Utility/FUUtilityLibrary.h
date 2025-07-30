// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "CoreMinimal.h"
#include "FUOrientedBox.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FUUtilityLibrary.generated.h"


/**
 * 
 */
UCLASS()
class FISHYUTILS_API UFUUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta=(DisplayName = "rotator + rotator", CompactNodeTitle = "+", ScriptMethod = "Add", ScriptOperator = "+;+=", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"), Category="Math|Rotator")
	static FRotator Add_RotatorRotator(FRotator A, FRotator B);

	UFUNCTION(BlueprintPure, meta=(DisplayName = "rotator - rotator", CompactNodeTitle = "-", ScriptMethod = "Subtract", ScriptOperator = "-;-=", Keywords = "- subtract minus"), Category="Math|Rotator")
	static FRotator Subtract_RotatorRotator(FRotator A, FRotator B);

	UFUNCTION(BlueprintCallable, Category="FishyUtils|Utility|Generic", meta=(WorldContext="WorldContextObject"))
	static bool AsyncOpenLevel(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> Level, bool bAbsolute = true, FString Options = "");
};
