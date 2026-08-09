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
	
	
	void GetAllGenerateOverlapEventsComponents(FU::Utils::FFUMessageBuilder& Builder);
	
	void GetAllGenerateOverlapEventsComponentsForWorld(FU::Utils::FFUMessageBuilder& Builder, UWorld* World);
	
	void GetAllGenerateOverlapEventsComponentsImpl(FU::Utils::FFUMessageBuilder& Builder, const TArray<const UPrimitiveComponent*>& Components);
	
	FU_CMD_RUNLAMBDA(GetAllGenerateOverlapEventsComponentsCmd, 
		"FU.Editor.GetAllGenerateOverlapEventsComponents", "",
		{
			FU::Utils::FFUMessageBuilder Builder;
			GetAllGenerateOverlapEventsComponents(Builder);
			FU_LOG_STemp_W("GetAllGenerateOverlapEventsComponents:\n{0}", *Builder.GetMessage());
		}
	);
	
	FU_CMD_RUNLAMBDA(GetAllGenerateOverlapEventsComponentsForCurrentWorldCmd, 
		"FU.Editor.GetAllGenerateOverlapEventsComponentsForCurrentWorld", "",
		{
			FU::Utils::FFUMessageBuilder Builder;
			GetAllGenerateOverlapEventsComponentsForWorld(Builder, GWorld);
			FU_LOG_STemp_W("GetAllGenerateOverlapEventsComponentsForCurrentWorld:\n{0}", *Builder.GetMessage());
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

