// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"


namespace FU_Utilities
{
	template<class ActorComponentType>
	ActorComponentType* SpawnRuntimeComponentForActor(AActor* Actor, TSubclassOf<ActorComponentType> ComponentClass = ActorComponentType::StaticClass()) 
	{
		static_assert(TIsDerivedFrom<ActorComponentType, UActorComponent>::IsDerived, "Provided type does not derive from UActorComponent");

		if (!IsValid(Actor) || !IsValid(ComponentClass)) { return nullptr; }
	
		EObjectFlags Flags = RF_Transient;
		Flags &= ~RF_Transactional; // unset flag
	
		ActorComponentType* NewComponent = NewObject<ActorComponentType>(Actor, ComponentClass, NAME_None, Flags);
		if (!IsValid(NewComponent)) { return nullptr; }
	
		NewComponent->RegisterComponent();
		Actor->AddInstanceComponent(NewComponent);
		
		return NewComponent;
	}
	
	FISHYUTILS_API void RemoveRuntimeComponent(UActorComponent* ActorComponent);

	/**
	 * @return "true" or "false"
	 */
	FISHYUTILS_API FString PrintBool(bool bVal);
	
	FISHYUTILS_API FString PrintCompactFloat(float Value);
	
	FISHYUTILS_API FString PrintCompactVector(FVector Value);

	FISHYUTILS_API FString PrintCompactRotator(FRotator Value);

	
	/**
	 *
	 *  @return "ObjectName"
	 *		- if the Object is an actor the label can be used if available
	 *		- if the Object is an actor component the name will be printed as "OwningActorName-ActorComponentName"
	 */
	FISHYUTILS_API FString GetObjectDetailedName(const UObject* Object);
	

	/**
	 *  Returns the last child
	 *  @return 'My.Sub.Tag' will return 'Tag'
	 */
	FISHYUTILS_API FString GetLastTagChild(const FGameplayTag& Tag);

	/**
	 *  Returns the x last child
	 *  @return 'My.Sub.Tag' will return 'Sub.Tag' if Depth=2
	 */
	FISHYUTILS_API FString GetLastTagChilds(const FGameplayTag& Tag, uint8 Depth);

	
	/**
	 *	For the given Component get the full chain of attachements.
	 *	@returns {Parent, Child1, Child2, ..., Component}
	 */
	FISHYUTILS_API void GetAttachChain(const USceneComponent* Component, TArray<const USceneComponent*>& OutChain);
	
	/**
	 *	For the given Component get the sum of all relative transforms. 
	 *  This can be used when world transform cannot be get directly on the Component.
	 */
	FISHYUTILS_API void GetAddedTransformStartingAtComponent(const USceneComponent* Component, FTransform& AddedTransform);
}
