// Copyright Flying Mass Dev Team. All Rights Reserved.

#pragma once

#include "FUSequencerUtils.generated.h" 
class ALevelSequenceActor;
class ULevelSequence;
class ULevelSequencePlayer;


USTRUCT()
struct FISHYUTILS_API FFUSequencerPlayParams
{
	GENERATED_BODY()
	
	FFUSequencerPlayParams();
	FFUSequencerPlayParams(UWorld* InWorld);

	// Core
	TWeakObjectPtr<UWorld> World;
	TWeakObjectPtr<ULevelSequence> LevelSequence;
	float PlayRate;
	int32 LoopCount;

	// Optional
	TOptional<FTransform> OverrideOriginInstanceTransform;
	TSubclassOf<ALevelSequenceActor> LevelSequenceActorClass;
};

USTRUCT()
struct FISHYUTILS_API FFUSequencerPlayResult
{
	GENERATED_BODY()
	
	TWeakObjectPtr<ALevelSequenceActor> LevelSequenceActor;
	TWeakObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;
};

namespace FU::Sequencer
{
	FISHYUTILS_API void PlaySequencer(const FFUSequencerPlayParams& Params, FFUSequencerPlayResult& OutResult);
}
