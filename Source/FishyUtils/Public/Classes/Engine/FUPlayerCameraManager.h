// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "FUPlayerCameraManager.generated.h"


enum class EFUCameraViewType
{
	None,
	Pitch,
	Yaw,
	Roll
};

/**
 * A snapshot of the player camera manager, used to restore the PCM view when an override is removed
 */
struct FISHYUTILS_API FFUCameraManagerSnapshot
{
	FFUCameraManagerSnapshot();

	void MakeSnapshot(EFUCameraViewType Type, AFUPlayerCameraManager* PCM);
	
	float ViewMin;
	float ViewMax;
};

/**
 * A camera view override which holds a snapshot of the modified view type beofer it was applied for easy restore.
 */
struct FISHYUTILS_API FFUCameraViewOverride
{
	FFUCameraViewOverride();
	FFUCameraViewOverride(EFUCameraViewType InType, float Min, float Max);

	void Apply(AFUPlayerCameraManager* PCM);
	void ResetToSnapshot(AFUPlayerCameraManager* PCM);
	
	FFUCameraManagerSnapshot Snapshot;
	EFUCameraViewType Type;
	TOptional<float> ViewMin;
	TOptional<float> ViewMax;

protected:
	void ApplyInternal(float* Value, float* PCMDest);
	float* GetPCMViewVarPtr(APlayerCameraManager* PCM, bool bGetMin);
};

/**
 * An entry of a given name, can hold 1 override per view type
 */
struct FISHYUTILS_API FFUCameraViewOverrideEntry
{
	FFUCameraViewOverrideEntry(AFUPlayerCameraManager* PCM, const FFUCameraViewOverride& Override);

	void AddOverride(AFUPlayerCameraManager* PCM, const FFUCameraViewOverride& Override);
	void RemoveOverride(AFUPlayerCameraManager* PCM, EFUCameraViewType Type);

	TMap<EFUCameraViewType, FFUCameraViewOverride> Overrides;
};

/**
 * Manages active overrides
 */
struct FISHYUTILS_API FFUCameraViewOverrideContainer
{
	FFUCameraViewOverrideContainer();
	FFUCameraViewOverrideContainer(AFUPlayerCameraManager* PCM);
	
	TWeakObjectPtr<AFUPlayerCameraManager> OwnerPCM;
	
	bool HasEntry(FName Name) const;

	/** Writes a snapshot of the PCM and applies the override */
	void AddEntry(FName Name, const FFUCameraViewOverride& Override);

	/** If found removed the entry of the given type and applies snapshot values */
	void RemoveEntry(FName Name, EFUCameraViewType Type);
	
protected:
	TMap<FName, FFUCameraViewOverrideEntry> ActiveEntries;
};


/**
 * Player Camera Manager from Fishy Utils
 */
UCLASS(DisplayName="Fishy Player Camera Manager")
class FISHYUTILS_API AFUPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
public:
	FFUCameraViewOverrideContainer ViewOverrideContainer;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	AFUPlayerCameraManager();
};
