// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "EngineUtils.h"
#include "Console/FUConsole.h"
#include "Framework/Commands/Commands.h"
#include "Logging/FULogging.h"
#include "Utility/FUUtilities.h"


namespace FU_EditorUtilities
{
	template<class ActorType>
	ActorType* GetEditorWorldCounterpartActor(ActorType* PIEActor)
	{
		static_assert(TIsDerivedFrom<ActorType, AActor>::IsDerived, "Provided type does not derive from AActor");

		// cant only use EditorUtilities::GetEditorWorldCounterpartActor because this breaks with level instances
		if (PIEActor->GetLevel()->IsPersistentLevel())
		{
			AActor* FoundEditorActor = EditorUtilities::GetEditorWorldCounterpartActor(PIEActor);
			if (ActorType* Actor = Cast<ActorType>(FoundEditorActor))
			{
				return Actor;
			}
		}
		else
		{
			UWorld* EditorWorld = GEditor->EditorWorld;
			for (auto LevelIt(EditorWorld->GetLevelIterator()); LevelIt; ++LevelIt)
			{
				if (const ULevel* Level = *LevelIt)
				{
					UWorld* World = CastChecked<UWorld>(Level->GetOuter());
					for (TActorIterator<ActorType> It(World); It; ++It)
					{
						if (It->GetActorLabel().Equals(PIEActor->GetActorLabel()))
						{
							return *It;
						}
					}
				}
			}
		}
		return nullptr;
	}
	
	
	/*----------------------------------------------------------------------------
		Overlap detection
	----------------------------------------------------------------------------*/
	enum EFUGenerateOverlapEventsResult
	{
		/** GenerateOverlapEvents is disabled */
		GenerateOverlapDisabled,
		/** GenerateOverlapEvents is enabled but the profile is set to NoCollision */
		NoCollision,
		/** GenerateOverlapEvents is enabled and Overlap is used as a Collision Response */
		OverlapCollisionResponseUsedOnly,
		/** GenerateOverlapEvents is enabled, Overlap is used as a Collision Response and Begin/End overlap delegates are bound */
		OverlapCollisionResponseUsedAndDelegatesBound,
		/** GenerateOverlapEvents is enabled and Begin/End overlap delegates are bound */
		DelegatesBoundOnly,
		/** GenerateOverlapEvents is enabled */
		GenerateOverlapEnabled
	};
	
	FString GenerateOverlapEventsResultToString(EFUGenerateOverlapEventsResult Result);
	
	
	void PrintAllGenerateOverlapEventsComponentsFromAssets();
	
	void PrintAllGenerateOverlapEventsComponentsFromWorld(UWorld* World);
	
	void PrintAllGenerateOverlapEventsComponentsFromPath(const FString& Path);
	
	EFUGenerateOverlapEventsResult GetGenerateOverlapEventsInfoForComponent(const UPrimitiveComponent* Component);
	
	bool ShouldGenerateOverlapEventsComponentBeIgnored(const UPrimitiveComponent* Component);
	
	void BuildMessageForGenerateOverlapEventsForComponent(const UPrimitiveComponent* Component, const FAssetData& Asset, EFUGenerateOverlapEventsResult Result);
	
	FU_CMD_RUNLAMBDA(GetAllGenerateOverlapEventsComponentsCmd, 
		"FU.Editor.PrintAllGenerateOverlapEventsComponentsFromAssets", "",
		{
			PrintAllGenerateOverlapEventsComponentsFromAssets();
		}
	);
	
	FU_CMD_RUNLAMBDA(PrintAllGenerateOverlapEventsComponentsFromWorldCmd, 
		"FU.Editor.PrintAllGenerateOverlapEventsComponentsFromCurrentWorld", "",
		{
			PrintAllGenerateOverlapEventsComponentsFromWorld(GWorld);
		}
	); 
	
	FU_CMD_RUNLAMBDA_WITHARGS(PrintAllGenerateOverlapEventsComponentsFromPathCmd, 
		"FU.Editor.PrintAllGenerateOverlapEventsComponentsFromPath", "",
		{
			if (Args.IsEmpty())
			{
				FU_LOG_STemp_W("Missing path argument (ex: SomeFolder = /Game/SomeFolder");
			}
			else
			{
				PrintAllGenerateOverlapEventsComponentsFromPath("/Game/" + Args[0]);
			}
		}
	);
	
	
	/*----------------------------------------------------------------------------
		Collision detection
	----------------------------------------------------------------------------*/
	void PrintAllCollisionEnabledComponentsFromWorld(UWorld* World, bool bHideActorsWithNoCollision);
	
	FU_CMD_RUNLAMBDA_WITHARGS(PrintAllCollisionEnabledComponentsFromWorldCmd, 
		"FU.Editor.PrintAllCollisionEnabledComponentsFromWorld", "Optionaly 1 to hide actors with no components with collisions enabled",
		{
			bool bHideActors = false;
			if (!Args.IsEmpty())
			{
				int32 Param1 = FCString::Atoi(*Args[0]);
				bHideActors = static_cast<bool>(Param1);
			}
			
			PrintAllCollisionEnabledComponentsFromWorld(GWorld, bHideActors);
		}
	); 
}


class FFUEditor
{
public:
	class FFUEditorCommands : public TCommands<FFUEditorCommands>
	{
	public:
		FFUEditorCommands() : TCommands<FFUEditorCommands>(
				"FishyEditorCommands",
				FText::FromString("Fishy Utilities"),
				NAME_None,
				FAppStyle::GetAppStyleSetName()
			)
		{}

		TSharedPtr<FUICommandInfo> SelectSameFolderLevel;

		virtual void RegisterCommands() override;
	};
	
public:
	FFUEditor();
	~FFUEditor();
	
	void RegisterMenuExtensions();
	
	void UnregisterMenuExtensions();

protected:
	void ExecuteSelectSameFolderLevel();
};

