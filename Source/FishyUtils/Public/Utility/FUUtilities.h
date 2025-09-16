// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"


namespace FU::Utils
{
	/** Wrap with WITH_EDITOR */
	#define FU_UTILS_EDITOR_RETURN_NOTGAMEWORLD if (!IsValid(GetWorld()) || !GetWorld()->IsGameWorld()) { return; }

	template<std::derived_from<UActorComponent> ActorComponentType>
	ActorComponentType* SpawnRuntimeComponentForActor(AActor* Actor, TSubclassOf<ActorComponentType> ComponentClass = ActorComponentType::StaticClass(), EObjectFlags Flags = RF_Transient) 
	{
		if (!IsValid(Actor) || !IsValid(ComponentClass)) { return nullptr; }
	
		Flags |= RF_Transient;
		Flags &= ~RF_Transactional; // unset flag in case it was set
	
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
	
	/* Print only the 2 decimals */
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
	 *	For the given TargetChildComponent get the full chain of attachements.
	 *	@returns {Parent, Child1, Child2, ..., TargetComponent}. Parent is usually the actor's default scene root component
	 */
	FISHYUTILS_API void GetAttachChain(USceneComponent* TargetChildComponent, TArray<USceneComponent*>& OutChain);
	FISHYUTILS_API void GetAttachChain(const USceneComponent* TargetChildComponent, TArray<const USceneComponent*>& OutChain);
	FISHYUTILS_API void GetAttachChain(USceneComponent* TargetChildComponent, TArray<TWeakObjectPtr<USceneComponent>>& OutChain);
	FISHYUTILS_API void GetAttachChain(const USceneComponent* TargetChildComponent, TArray<TWeakObjectPtr<const USceneComponent>>& OutChain);

#if WITH_EDITOR
	/**
	 *  The SCS must be from the owning actor of the TargetChildComponent.
	 *  This iterarates by starting at the root nodes (aka the root component of the owning actor of the SCS).
	 *	@returns {Parent, Child1, Child2, ..., TargetComponent}
	 */
	FISHYUTILS_API void GetAttachChainForChildComponent_BlueprintEditor(const USceneComponent* TargetChildComponent, const USimpleConstructionScript* SCS, TArray<const USceneComponent*>& OutChain);
	FISHYUTILS_API void GetAttachChainForChildComponent_BlueprintEditor(const USceneComponent* TargetChildComponent, const USimpleConstructionScript* SCS, TArray<TWeakObjectPtr<const USceneComponent>>& OutChain);
	
	/* Used internally by GetAttachChainForChildComponent_BlueprintEditor */
	FISHYUTILS_API bool GetAttachChainForChildComponent_BlueprintEditor_IterateNodes(const USceneComponent* TargetComponent, const USCS_Node* CurrentParentNode, TArray<const USCS_Node*>& OutPath);
#endif
	
	/**
	 *	For the given Component get the sum of all relative transforms. 
	 *  This can be used when world transform cannot be get directly on the Component.
	 */
	FISHYUTILS_API void GetSimulatedWorldTransformFromComponent(const USceneComponent* Component, FTransform& OutWorldTransform, TArray<const USceneComponent*>* OverrideChain = nullptr);
	FISHYUTILS_API void GetSimulatedWorldTransformFromComponent(USceneComponent* Component, FTransform& OutWorldTransform, TArray<USceneComponent*>* OverrideChain = nullptr);
	FISHYUTILS_API void GetSimulatedWorldTransformFromComponent(const USceneComponent* Component, FTransform& OutWorldTransform, TArray<TWeakObjectPtr<const USceneComponent>>* OverrideChain = nullptr);

	/* Remove the _GEN_VARIABLE part of the name */
	FISHYUTILS_API FName GetFNameWithoutTemplateSuffix(FName ObjectName);
	FISHYUTILS_API FString GetNameWithoutTemplateSuffix(FName ObjectName);
	FISHYUTILS_API FName GetFNameWithoutTemplateSuffix(UObject* Object);
	FISHYUTILS_API FString GetNameWithoutTemplateSuffix(UObject* Object);

	/* Remove the _C part of the name */
	FISHYUTILS_API FName GetFNameWithoutClassSuffix(FName ClassName);
	FISHYUTILS_API FString GetNameWithoutClassSuffix(FName ClassName);
	FISHYUTILS_API FName GetFNameWithoutClassSuffix(UClass* Class);
	FISHYUTILS_API FString GetNameWithoutClassSuffix(UClass* Class);

	namespace Loading
	{
		/**
		 * Use with FU_UTILS_ASYNCLOAD inside the callback to know if the UObject that requested the async load is still valid
		 */
		#define FU_UTILS_ASYNCLOAD_SELF_VALID IsValid(this)

		/**
		 * Use with FU_UTILS_ASYNCLOAD inside the callback to know if the load suceeded in a valid UObject
		 */
		#define FU_UTILS_ASYNCLOAD_RES_BOOL bIsValidLoad

		/**
		 * Use with FU_UTILS_ASYNCLOAD inside the callback to get the loaded UObject
		 */
		#define FU_UTILS_ASYNCLOAD_RES_OBJECT LoadedObject

		/**
		 * Wrapper for async requests.
		 * At the end of the async request we arent assigning the loaded object to the destination ptr if "this" is invalid.
		 * Same for the callback body.
		 * You cannot cancel this request.
		 * 
		 * Example of use with callback:
		 * @code
		 * // TSoftClassPtr<UGameplayAbility> ClimbAbilityClass;
		 * // UPROPERTY() TObjectPtr<UClass> LoadedClimbAbilityClass;
		 * FU_ASYNCLOAD(ClimbAbilityClass.ToSoftObjectPath(), UClass, LoadedClimbAbilityClass, 0,
		 * {
		 *    if (FU_UTILS_ASYNCLOAD_RES_BOOL)
		 *	   {
		 *	      GameplayAbilitySpecHandle = OwnerAbilitySystemComponent->K2_GiveAbility(LoadedClimbAbilityClass, 0, -1);
		 *	  }
		 * });
		 *  @endcode 
		 */
		#define FU_UTILS_ASYNCLOAD(SoftPath, Class, DestinationOnLoad, Priority, CallbackBody, ...) \
			{ \
				auto OnLoaded = [this, ##__VA_ARGS__](const FSoftObjectPath& Path, Class* FU_UTILS_ASYNCLOAD_RES_OBJECT) \
				{ \
					const bool FU_UTILS_ASYNCLOAD_RES_BOOL = IsValid(FU_UTILS_ASYNCLOAD_RES_OBJECT); \
					if (FU_UTILS_ASYNCLOAD_SELF_VALID) \
					{ \
						if (FU_UTILS_ASYNCLOAD_RES_BOOL) \
						{ \
							DestinationOnLoad = FU_UTILS_ASYNCLOAD_RES_OBJECT; \
						} \
						CallbackBody; \
					} \
				}; \
				FLoadAssetAsyncOptionalParams LoadAssetAsyncOptionalParams; \
				LoadAssetAsyncOptionalParams.PackagePriority = Priority; \
				SoftPath.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateLambda([this, OnLoaded](const FSoftObjectPath& Path, UObject* LoadedObject) \
				{ \
					OnLoaded(Path, Cast<Class>(LoadedObject)); \
				}), LoadAssetAsyncOptionalParams); \
			} \

		/**
		 * Similar to FU_UTILS_ASYNCLOAD but you have to handle the assignement yourself in the callback.
		 */
		#define FU_UTILS_ASYNCLOAD_NOASSIGN(Id, SoftPath, Class, Priority, CallbackBody, ...) \
			{ \
				auto OnLoaded = [this, ##__VA_ARGS__](const FSoftObjectPath& Path, Class* FU_UTILS_ASYNCLOAD_RES_OBJECT) \
				{ \
					const bool FU_UTILS_ASYNCLOAD_RES_BOOL = IsValid(FU_UTILS_ASYNCLOAD_RES_OBJECT); \
					if (FU_UTILS_ASYNCLOAD_SELF_VALID) \
					{ \
						CallbackBody; \
					} \
				}; \
				FLoadAssetAsyncOptionalParams LoadAssetAsyncOptionalParams; \
				LoadAssetAsyncOptionalParams.PackagePriority = Priority; \
				SoftPath.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateLambda([this, OnLoaded](const FSoftObjectPath& Path, UObject* LoadedObject) \
				{ \
					OnLoaded(Path, Cast<Class>(LoadedObject)); \
				}), LoadAssetAsyncOptionalParams); \
			} \
		
		template<std::derived_from<UObject> LoadedObjectType>
		bool SyncLoadObjects(const TArray<FSoftObjectPath>& SoftPaths, TArray<LoadedObjectType*>& LoadedObjects)
		{
			if (SoftPaths.IsEmpty()) { return false; }
			
			for (auto& SoftPath : SoftPaths)
			{
				if (!SoftPath.IsNull())
				{
					auto& NewEntry = LoadedObjects.Emplace_GetRef(nullptr);
					if (!SyncLoadObject<LoadedObjectType>(SoftPath, NewEntry))
					{
						LoadedObjects.RemoveAt(LoadedObjects.Num()-1);
						return false;
					}
				}
			}
			return true;
		}
		
		template<std::derived_from<UObject> LoadedObjectType>
		bool SyncLoadObject(const FSoftObjectPath& SoftPath, LoadedObjectType*& LoadedObject)
		{
			if (SoftPath.IsNull()) { return false; }

			auto* AM = UAssetManager::GetIfInitialized();
			if (!IsValid(AM)) { return false; }

			LoadedObject = Cast<LoadedObjectType>(AM->GetStreamableManager().LoadSynchronous(SoftPath));

			return true;
		}
	}
}
