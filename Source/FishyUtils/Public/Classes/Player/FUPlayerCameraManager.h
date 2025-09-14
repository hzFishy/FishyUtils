// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "FUPlayerCameraManager.generated.h"


enum class EFUCameraViewType
{
	None,
	Pitch,
	Yaw,
	Roll
};

struct FISHYUTILS_API FFUCameraManagerSnapshot
{
	FFUCameraManagerSnapshot();

	void MakeSnapshot(EFUCameraViewType Type, AFUPlayerCameraManager* PCM);
	
	float ViewMin;
	float ViewMax;
};

struct FISHYUTILS_API FFUCameraViewOverride
{
	FFUCameraViewOverride();
	FFUCameraViewOverride(EFUCameraViewType InType, float Min, float Max);

	void Apply(AFUPlayerCameraManager* PCM);
	void ResetToSnapshot(AFUPlayerCameraManager* PCM);
	
	// state before being applied
	FFUCameraManagerSnapshot Snapshot;
	EFUCameraViewType Type;
	TOptional<float> ViewMin;
	TOptional<float> ViewMax;

protected:
	void ApplyInternal(float* Value, float* PCMDest);
	float* GetPCMViewVarPtr(APlayerCameraManager* PCM, bool bGetMin);
};

struct FISHYUTILS_API FFUCameraViewOverrideEntry
{
	FFUCameraViewOverrideEntry(AFUPlayerCameraManager* PCM, const FFUCameraViewOverride& Override);

	void AddOverride(AFUPlayerCameraManager* PCM, const FFUCameraViewOverride& Override);
	void RemoveOverride(AFUPlayerCameraManager* PCM, EFUCameraViewType Type);

	TMap<EFUCameraViewType, FFUCameraViewOverride> Overrides;
};

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
 * 
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
