// Copyright Prison Mars Dev Team. All Rights Reserved.


#include "AI/StateTree/FUSTC_GameplayTagCompare.h"
#include "StateTreeExecutionContext.h"


FFUSTC_GameplayTagCompare::FFUSTC_GameplayTagCompare(): 
	bExactMatch(false), 
	bInvert(false)
{}

bool FFUSTC_GameplayTagCompare::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	bool bResult;
	
	if (bExactMatch)
	{
		bResult = InstanceData.TagA.MatchesTagExact(InstanceData.TagB);
	}
	else
	{
		bResult = InstanceData.TagA.MatchesTag(InstanceData.TagB);
	}
	
	return bInvert ? !bResult : bResult;
}

#if WITH_EDITOR
FText FFUSTC_GameplayTagCompare::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType& InstanceData = InstanceDataView.Get<FInstanceDataType>();
	
	const FString Result = FString::Printf(TEXT("GameplayTag Compare (%s) (%s) %s"),
		*InstanceData.TagB.ToString(),
		*FString(bExactMatch ? "Exact" : "NotExact"),
		*FString(bInvert ? "(Inverted)" : "")
	);
	return FText::FromString(Result);
}
#endif
