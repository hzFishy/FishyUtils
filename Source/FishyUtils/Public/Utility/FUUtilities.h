﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameplayTagContainer.h"


namespace FU_Utilities
{
	/**
	 * @return "true" or "false"
	 */
	FISHYUTILS_API FString PrintBool(bool bVal);
	
	FISHYUTILS_API FString PrintShortFloat(float Value);
	
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

}
