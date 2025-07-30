// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "EngineUtils.h"
#include "Framework/Commands/Commands.h"


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

