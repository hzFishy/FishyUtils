// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/FUUtilities.h"
#include "Algo/Count.h"


void FU_Utilities::RemoveRuntimeComponent(UActorComponent* ActorComponent)
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

FString FU_Utilities::PrintCompactFloat(float Value)
{
	return FString::Printf(TEXT("%.2f"), Value);
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


void FU_Utilities::GetAttachChain(const USceneComponent* Component, TArray<const USceneComponent*>& OutChain)
{
	OutChain.Insert(Component, 0);
	
	if (IsValid(Component->GetAttachParent()))
	{
		GetAttachChain(Component->GetAttachParent(), OutChain);
	}
}

void FU_Utilities::GetAddedTransformStartingAtComponent(const USceneComponent* Component, FTransform& AddedTransform)
{
	TArray<const USceneComponent*> OutChain;
	GetAttachChain(Component, OutChain);

	for (int i = 0; i < OutChain.Num(); ++i)
	{
		const auto* CurrentComponent = OutChain[i];

		AddedTransform.SetLocation(AddedTransform.GetLocation() + CurrentComponent->GetRelativeLocation());

		//AddedTransform.SetRotation((AddedTransform.GetRotation().Rotator() + Component->GetRelativeRotation()).Quaternion());
	
		UE::Math::TQuat<double> NewQuat = AddedTransform.GetRotation() * CurrentComponent->GetRelativeRotation().Quaternion();
		NewQuat.Normalize();
		AddedTransform.SetRotation(NewQuat);
	
		AddedTransform.SetScale3D(AddedTransform.GetScale3D() * CurrentComponent->GetRelativeScale3D());

	}
}
