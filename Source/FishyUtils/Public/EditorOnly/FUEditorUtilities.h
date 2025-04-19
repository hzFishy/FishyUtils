// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


namespace FU_EditorUtilities
{
	
#if WITH_EDITOR
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
#endif
	
}
