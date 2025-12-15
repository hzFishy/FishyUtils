// Copyright Flying Mass Dev Team. All Rights Reserved.


#include "Utility/FUSequencerUtils.h"

#include "DefaultLevelSequenceInstanceData.h"
#include "LevelSequencePlayer.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"


FFUSequencerPlayParams::FFUSequencerPlayParams():
	PlayRate(1),
	LoopCount(0)
{
	LevelSequenceActorClass = ALevelSequenceActor::StaticClass();
}

FFUSequencerPlayParams::FFUSequencerPlayParams(UWorld* InWorld):
	World(InWorld),
	PlayRate(1),
	LoopCount(0)
{
	LevelSequenceActorClass = ALevelSequenceActor::StaticClass();
}


namespace FU::Sequencer
{
	void PlaySequencer(const FFUSequencerPlayParams& Params, FFUSequencerPlayResult& OutResult)
	{
		if (!Params.World.IsValid()) { return; }
		if (!Params.LevelSequence.IsValid()) { return; }
		if (!IsValid(Params.LevelSequenceActorClass)) { return; }
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.ObjectFlags |= RF_Transient;
		SpawnParams.bAllowDuringConstructionScript = true;

		// Defer construction for autoplay so that BeginPlay() is called
		SpawnParams.bDeferConstruction = true;

		OutResult.LevelSequenceActor = Params.World->SpawnActor<ALevelSequenceActor>(Params.LevelSequenceActorClass, SpawnParams);

		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		PlaybackSettings.bAutoPlay = true;
		PlaybackSettings.PlayRate = Params.PlayRate;
		PlaybackSettings.LoopCount.Value = Params.LoopCount;
		OutResult.LevelSequenceActor->PlaybackSettings = PlaybackSettings;
		
		OutResult.LevelSequencePlayer = OutResult.LevelSequenceActor->GetSequencePlayer();
		
		OutResult.LevelSequencePlayer->SetPlaybackSettings(PlaybackSettings);

		OutResult.LevelSequenceActor->SetSequence(Params.LevelSequence.Get());

		if (Params.OverrideOriginInstanceTransform.IsSet())
		{
			OutResult.LevelSequenceActor->bOverrideInstanceData = true;
			auto* LevelSequenceInstanceData = Cast<UDefaultLevelSequenceInstanceData>(OutResult.LevelSequenceActor->DefaultInstanceData);
			LevelSequenceInstanceData->TransformOrigin = Params.OverrideOriginInstanceTransform.GetValue();
		}
	
		OutResult.LevelSequenceActor->InitializePlayer();

		FTransform DefaultTransform;
		OutResult.LevelSequenceActor->FinishSpawning(DefaultTransform);
	}
}


