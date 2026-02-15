// Copyright Prison Mars Dev Team. All Rights Reserved.

#pragma once

#include "StateTreeConditionBase.h"
#include "FUSTC_GameplayTagCompare.generated.h"

USTRUCT()
struct FFUSTC_GameplayTagCompareInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	FGameplayTag TagA;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FGameplayTag TagB;
};

/**
 * Condition comparing two gameplay tags.
 */
USTRUCT(DisplayName="GameplayTag Compare", Category="FishyUtils")
struct FFUSTC_GameplayTagCompare : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FFUSTC_GameplayTagCompareInstanceData;

	FFUSTC_GameplayTagCompare();
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
	
	UPROPERTY(EditAnywhere, Category="Parameter")
	bool bExactMatch;
	
	UPROPERTY(EditAnywhere, Category="Parameter")
	bool bInvert;	
};
