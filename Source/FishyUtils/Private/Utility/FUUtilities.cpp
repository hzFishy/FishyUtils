// By hzFishy 2025 - Do whatever you want with it


#include "Utility/FUUtilities.h"
#include "Algo/Count.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"


void FU::Utils::RemoveRuntimeComponent(UActorComponent* ActorComponent)
{
	if (IsValid(ActorComponent))
	{
		ActorComponent->GetOwner()->RemoveInstanceComponent(ActorComponent);
		ActorComponent->UnregisterComponent();
	}
}

FString FU::Utils::PrintBool(bool bValue)
{
	return bValue ? "true" : "false";
}

FString FU::Utils::PrintCompactFloat(float Value)
{
	return FString::Printf(TEXT("%.2f"), Value);
}

FString FU::Utils::PrintCompactFloat(const float* Value)
{
	if (Value == nullptr)
	{
		return "Null Float";
	}
	else
	{
		return FU::Utils::PrintCompactFloat(*Value);
	}
}

FString FU::Utils::PrintCompactVector(FVector Value)
{
	return FString::Printf(TEXT("(X=%.1f, Y=%.1f, Z=%.1f)"), Value.X, Value.Y, Value.Z);
}

FString FU::Utils::PrintCompactRotator(FRotator Value)
{
	return FString::Printf(TEXT("(R/X=%.1f, P/Y=%.1f, Y/Z=%.1f)"), Value.Roll, Value.Pitch, Value.Yaw);
}

FString FU::Utils::SafeGetName(const UObject* Object)
{
	if (IsValid(Object))
	{
		return Object->GetName();
	}
	return "Null Object";
}


FString FU::Utils::GetObjectDetailedName(const UObject* Object)
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


FString FU::Utils::GetLastTagChild(const FGameplayTag& Tag)
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

FString FU::Utils::GetLastTagChilds(const FGameplayTag& Tag, uint8 Depth)
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


void FU::Utils::GetAttachChain(USceneComponent* TargetChildComponent, TArray<USceneComponent*>& OutChain)
{
	OutChain.Insert(TargetChildComponent, 0);
	
	auto* ParentComponent = TargetChildComponent->GetAttachParent();
	if (ParentComponent != nullptr && ParentComponent->GetFName().IsValid())
	{
		GetAttachChain(ParentComponent, OutChain);
	}
}

void FU::Utils::GetAttachChain(const USceneComponent* TargetChildComponent, TArray<const USceneComponent*>& OutChain)
{
	OutChain.Insert(TargetChildComponent, 0);
	
	const auto* ParentComponent = TargetChildComponent->GetAttachParent();
	if (ParentComponent != nullptr && ParentComponent->GetFName().IsValid())
	{
		GetAttachChain(ParentComponent, OutChain);
	}
}

void FU::Utils::GetAttachChain(USceneComponent* TargetChildComponent, TArray<TWeakObjectPtr<USceneComponent>>& OutChain)
{
	OutChain.Insert(TargetChildComponent, 0);
	
	auto* ParentComponent = TargetChildComponent->GetAttachParent();
	if (ParentComponent != nullptr && ParentComponent->GetFName().IsValid())
	{
		GetAttachChain(ParentComponent, OutChain);
	}
}

void FU::Utils::GetAttachChain(const USceneComponent* TargetChildComponent, TArray<TWeakObjectPtr<const USceneComponent>>& OutChain)
{
	OutChain.Insert(TargetChildComponent, 0);
	
	const auto* ParentComponent = TargetChildComponent->GetAttachParent();
	if (ParentComponent != nullptr && ParentComponent->GetFName().IsValid())
	{
		GetAttachChain(ParentComponent, OutChain);
	}
}

#if WITH_EDITOR
void FU::Utils::GetAttachChainForChildComponent_BlueprintEditor(const USceneComponent* TargetChildComponent, const USimpleConstructionScript* SCS,
	TArray<const USceneComponent*>& OutChain)
{
	if (TargetChildComponent->HasAnyFlags(EObjectFlags::RF_ArchetypeObject))
	{
		// iterate root node, then go for childs, sub childs ... etc until finding the component
		for (auto& RootNode : SCS->GetRootNodes())
		{
			TArray<const USCS_Node*> OutPathNodes;
			OutPathNodes.Add(RootNode);
			if (GetAttachChainForChildComponent_BlueprintEditor_IterateNodes(TargetChildComponent, RootNode, OutPathNodes))
			{
				for (auto& OutPathNode : OutPathNodes)
				{
					OutChain.Add(Cast<USceneComponent>(OutPathNode->ComponentTemplate));
				}
				break;
			}
		}
		return;
	}
	else
	{
		GetAttachChain(TargetChildComponent, OutChain);
	}
}

void FU::Utils::GetAttachChainForChildComponent_BlueprintEditor(const USceneComponent* TargetChildComponent,
	const USimpleConstructionScript* SCS, TArray<TWeakObjectPtr<const USceneComponent>>& OutChain)
{
	if (TargetChildComponent->HasAnyFlags(EObjectFlags::RF_ArchetypeObject))
	{
		// iterate root node, then go for childs, sub childs ... etc until finding the component
		for (auto& RootNode : SCS->GetRootNodes())
		{
			TArray<const USCS_Node*> OutPathNodes;
			OutPathNodes.Add(RootNode);
			if (GetAttachChainForChildComponent_BlueprintEditor_IterateNodes(TargetChildComponent, RootNode, OutPathNodes))
			{
				for (auto& OutPathNode : OutPathNodes)
				{
					OutChain.Add(Cast<USceneComponent>(OutPathNode->ComponentTemplate));
				}
				break;
			}
		}
		return;
	}
	else
	{
		GetAttachChain(TargetChildComponent, OutChain);
	}
}

bool FU::Utils::GetAttachChainForChildComponent_BlueprintEditor_IterateNodes(const USceneComponent* TargetComponent, const USCS_Node* CurrentParentNode, TArray<const USCS_Node*>& OutPath)
{
	// check if we hit the target node
	if (CurrentParentNode->ComponentTemplate->GetFName() == TargetComponent->GetFName()) { return true; }

	for (auto& ChildNode : CurrentParentNode->GetChildNodes())
	{
		// add current node in path
		OutPath.Add(ChildNode);
	
		if (GetAttachChainForChildComponent_BlueprintEditor_IterateNodes(TargetComponent, ChildNode, OutPath))
		{
			return true;
		}

		// we didnt find it, remove as this path is invalid
		OutPath.RemoveAt(OutPath.Num() - 1);
	}

	return false;
}

#endif

void FU::Utils::GetSimulatedWorldTransformFromComponent(const USceneComponent* Component, FTransform& OutWorldTransform, TArray<const USceneComponent*>* OverrideChain)
{
	TArray<const USceneComponent*> OutChain;
	if (OverrideChain != nullptr)
	{
		OutChain = *OverrideChain;
	}
	else
	{
		GetAttachChain(Component, OutChain);
	}
	
	for (int i = 0; i < OutChain.Num(); ++i)
	{
		const auto* CurrentComponent = OutChain[i];

		const auto& ParentQuat = OutWorldTransform.GetRotation();
		const FVector& ParentLocation = OutWorldTransform.GetLocation();
		const FVector& ParentScale = OutWorldTransform.GetScale3D();
		const auto& RelativeQuat = CurrentComponent->GetRelativeRotation().Quaternion();
		const FVector& RelativeLocation = CurrentComponent->GetRelativeTransform().GetLocation();

		// location, takes into account rotation and scale
		OutWorldTransform.SetLocation(ParentLocation + (ParentQuat.RotateVector(RelativeLocation * ParentScale)));

		// rotation
		OutWorldTransform.SetRotation((OutWorldTransform.GetRotation() * RelativeQuat).GetNormalized());

		// scale
		OutWorldTransform.SetScale3D(OutWorldTransform.GetScale3D() * CurrentComponent->GetRelativeScale3D());
	}
}

void FU::Utils::GetSimulatedWorldTransformFromComponent(USceneComponent* Component, FTransform& OutWorldTransform,
	TArray<USceneComponent*>* OverrideChain)
{
	TArray<USceneComponent*> OutChain;
	if (OverrideChain != nullptr)
	{
		OutChain = *OverrideChain;
	}
	else
	{
		GetAttachChain(Component, OutChain);
	}
	
	for (int i = 0; i < OutChain.Num(); ++i)
	{
		auto* CurrentComponent = OutChain[i];

		const auto& ParentQuat = OutWorldTransform.GetRotation();
		const FVector& ParentLocation = OutWorldTransform.GetLocation();
		const FVector& ParentScale = OutWorldTransform.GetScale3D();
		const auto& RelativeQuat = CurrentComponent->GetRelativeRotation().Quaternion();
		const FVector& RelativeLocation = CurrentComponent->GetRelativeTransform().GetLocation();

		// location, takes into account rotation and scale
		OutWorldTransform.SetLocation(ParentLocation + (ParentQuat.RotateVector(RelativeLocation * ParentScale)));

		// rotation
		OutWorldTransform.SetRotation((OutWorldTransform.GetRotation() * RelativeQuat).GetNormalized());

		// scale
		OutWorldTransform.SetScale3D(OutWorldTransform.GetScale3D() * CurrentComponent->GetRelativeScale3D());
	}
}

void FU::Utils::GetSimulatedWorldTransformFromComponent(const USceneComponent* Component,
	FTransform& OutWorldTransform, TArray<TWeakObjectPtr<const USceneComponent>>* OverrideChain)
{
	TArray<TWeakObjectPtr<const USceneComponent>> OutChain;
	if (OverrideChain != nullptr)
	{
		OutChain = *OverrideChain;
	}
	else
	{
		GetAttachChain(Component, OutChain);
	}
	
	for (int i = 0; i < OutChain.Num(); ++i)
	{
		auto* CurrentComponent = OutChain[i].Get();

		const auto& ParentQuat = OutWorldTransform.GetRotation();
		const FVector& ParentLocation = OutWorldTransform.GetLocation();
		const FVector& ParentScale = OutWorldTransform.GetScale3D();
		const auto& RelativeQuat = CurrentComponent->GetRelativeRotation().Quaternion();
		const FVector& RelativeLocation = CurrentComponent->GetRelativeTransform().GetLocation();

		// location, takes into account rotation and scale
		OutWorldTransform.SetLocation(ParentLocation + (ParentQuat.RotateVector(RelativeLocation * ParentScale)));

		// rotation
		OutWorldTransform.SetRotation((OutWorldTransform.GetRotation() * RelativeQuat).GetNormalized());

		// scale
		OutWorldTransform.SetScale3D(OutWorldTransform.GetScale3D() * CurrentComponent->GetRelativeScale3D());
	}
}

FName FU::Utils::GetFNameWithoutTemplateSuffix(FName ObjectName)
{
	FString StringName = ObjectName.ToString();

	StringName.RemoveFromEnd("_GEN_VARIABLE");
	
	return FName(*StringName);
}

FString FU::Utils::GetNameWithoutTemplateSuffix(FName ObjectName)
{
	return GetFNameWithoutTemplateSuffix(ObjectName).ToString();
}

FName FU::Utils::GetFNameWithoutTemplateSuffix(UObject* Object)
{
	return GetFNameWithoutTemplateSuffix(Object->GetFName());
}

FString FU::Utils::GetNameWithoutTemplateSuffix(UObject* Object)
{
	return GetNameWithoutTemplateSuffix(Object->GetFName());
}

FName FU::Utils::GetFNameWithoutClassSuffix(FName ClassName)
{
	FString StringName = ClassName.ToString();

	StringName.RemoveFromEnd("_C");
	
	return FName(*StringName);
}
FString FU::Utils::GetNameWithoutClassSuffix(FName ClassName)
{
	return GetFNameWithoutClassSuffix(ClassName).ToString();
}

FName FU::Utils::GetFNameWithoutClassSuffix(UClass* Class)
{
	return GetFNameWithoutClassSuffix(Class->GetFName());
}

FString FU::Utils::GetNameWithoutClassSuffix(UClass* Class)
{
	return GetNameWithoutClassSuffix(Class->GetFName());
}


FU::Utils::FFUMessageBuilder::FFUMessageBuilder() {}

FU::Utils::FFUMessageBuilder::FFUMessageBuilder(FString InitalText)
{
	Append(InitalText);
}

FU::Utils::FFUMessageBuilder& FU::Utils::FFUMessageBuilder::Append(FString Text)
{
	Message.Append(Text);
	return *this;
}

FU::Utils::FFUMessageBuilder& FU::Utils::FFUMessageBuilder::Append(bool bCond, FString Text)
{
	if (bCond)
	{
		return Append(Text);
	}
	return *this;
}

FU::Utils::FFUMessageBuilder& FU::Utils::FFUMessageBuilder::NewLine(FString Text)
{
	return Append("\n" + Text);
}

FU::Utils::FFUMessageBuilder& FU::Utils::FFUMessageBuilder::NewLine(bool bCond, FString Text)
{
	if (bCond)
	{
		return NewLine(Text);
	}
	return *this;
}


bool FU::Utils::Math::IsPointInSphere(const FVector& Center, float Radius, const FVector& Point)
{
	return (Point - Center).Length() < Radius;
}
