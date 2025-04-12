// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/FUUtilities.h"
#include "Algo/Count.h"


void FU_Utilities::RemoveRuntimeInstanceComponent(UActorComponent* ActorComponent)
{
	if (IsValid(ActorComponent))
	{
		ActorComponent->GetOwner()->RemoveInstanceComponent(ActorComponent);
		ActorComponent->UnregisterComponent();
	}
}

FString FU_Utilities::PrintBool(bool bValue)
{
	return bValue ? "true" : "false";
}

FString FU_Utilities::PrintShortFloat(float Value)
{
	return FString::Printf(TEXT("%.1f"), Value);
}

FString FU_Utilities::PrintCompactVector(FVector Value)
{
	return FString::Printf(TEXT("(X=%.1f, Y=%.1f, Z=%.1f)"), Value.X, Value.Y, Value.Z);
}

FString FU_Utilities::PrintCompactRotator(FRotator Value)
{
	return FString::Printf(TEXT("(R/X=%.1f, P/Y=%.1f, Y/Z=%.1f)"), Value.Roll, Value.Pitch, Value.Yaw);
}


FString FU_Utilities::GetObjectDetailedName(const UObject* Object)
{
	if (IsValid(Object))
	{
		FString BaseString = "";

		if (const AActor* Actor = Cast<AActor>(Object))
		{
			// if we are an actor in dev build we can get the label
			
			BaseString += Actor->GetActorNameOrLabel();
		}
		else if (const UActorComponent* Component = Cast<UActorComponent>(Object))
		{
			// if we are an actor component we combine to ActorName-ComponentName
			
			const AActor* OwningActor = Component->GetOwner();
			if (OwningActor)
			{
				BaseString += OwningActor->GetActorNameOrLabel();
				BaseString += "-";
			}
				
			BaseString += Object->GetName();
		}
		else
		{
			BaseString += Object->GetName();
		}
		return BaseString;
	}
	else
	{
		return "InvalidObject";
	}
}


FString FU_Utilities::GetLastTagChild(const FGameplayTag& Tag)
{
	if (!Tag.IsValid()) { return "None"; }
	
	const FString& TagString = Tag.ToString();
	uint32 FoundIndex = TagString.Find(".", ESearchCase::Type::IgnoreCase, ESearchDir::Type::FromEnd);
	if (FoundIndex > 0)
	{
		return TagString.Right(TagString.Len() - FoundIndex - 1);
	}
	
	return TagString;
}

FString FU_Utilities::GetLastTagChilds(const FGameplayTag& Tag, uint8 Depth)
{
	if (!Tag.IsValid()) { return "None"; }

	// check if the asked depth is not bigger than the number of subtag
	check(Depth <= Algo::Count(Tag.ToString(), TEXT('.')) + 1)

	FGameplayTag CurrentItTag = Tag;
	FString Result = "";

	for (int i = 1; i < Depth+1; ++i)
	{
		if (i == 1)
		{
			Result = GetLastTagChild(CurrentItTag);
		}
		else
		{
			// cache new tag (the parent)
			CurrentItTag = Tag.RequestDirectParent();

			// if we were at the root "RequestDirectParent" will return an empty GT
			if (!CurrentItTag.IsValid()) { break; }
			
			// Previous.Next format
			Result = FString::Printf(TEXT("%s.%s"), *GetLastTagChild(CurrentItTag), *Result);
		}
	}
	
	return Result;
}