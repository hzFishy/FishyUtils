// By hzFishy - 2026 - Do whatever you want with it.


#include "Classes/Engine/FUPlayerCameraManager.h"
#include "Asserts/FUAsserts.h"


FFUCameraManagerSnapshot::FFUCameraManagerSnapshot():
	ViewMin(0),
	ViewMax(0)
{}

void FFUCameraManagerSnapshot::MakeSnapshot(EFUCameraViewType Type, AFUPlayerCameraManager* PCM)
{
	if (!IsValid(PCM)) { return; }

	switch (Type)
	{
		case EFUCameraViewType::Pitch:
			{
				ViewMin = PCM->ViewPitchMin;
				ViewMax = PCM->ViewPitchMax;
				break;
			}
		case EFUCameraViewType::Yaw:
			{
				ViewMin = PCM->ViewYawMin;
				ViewMax = PCM->ViewYawMax;
				break;
			}
		case EFUCameraViewType::Roll:
			{
				ViewMin = PCM->ViewRollMin;
				ViewMax = PCM->ViewRollMax;
				break;
			}
		default:
			break;
	}
}


FFUCameraViewOverride::FFUCameraViewOverride(): Type(EFUCameraViewType::None)
{}

FFUCameraViewOverride::FFUCameraViewOverride(EFUCameraViewType InType, float Min, float Max):
	Type(InType),
	ViewMin(Min),
	ViewMax(Max)
{}

void FFUCameraViewOverride::Apply(AFUPlayerCameraManager* PCM)
{
	if (!IsValid(PCM)) { return; }
	if (Type == EFUCameraViewType::None) { return; }

	if (auto* MinVal = ViewMin.GetPtrOrNull())
	{
		ApplyInternal(MinVal, GetPCMViewVarPtr(PCM, true));
	}
	
	if (auto* MaxVal = ViewMax.GetPtrOrNull())
	{
		ApplyInternal(MaxVal, GetPCMViewVarPtr(PCM, false));
	}
}

void FFUCameraViewOverride::ResetToSnapshot(AFUPlayerCameraManager* PCM)
{
	ApplyInternal(&Snapshot.ViewMin, GetPCMViewVarPtr(PCM, true));
	ApplyInternal(&Snapshot.ViewMax, GetPCMViewVarPtr(PCM, false));
}

void FFUCameraViewOverride::ApplyInternal(float* Value, float* PCMDest)
{
	*PCMDest = *Value;
}

float* FFUCameraViewOverride::GetPCMViewVarPtr(APlayerCameraManager* PCM, bool bGetMin)
{
	switch (Type)
	{
		case EFUCameraViewType::Pitch:
			{
				return bGetMin ? &PCM->ViewPitchMin : &PCM->ViewPitchMax;
			}
		case EFUCameraViewType::Yaw:
			{
				return bGetMin ? &PCM->ViewYawMin : &PCM->ViewYawMax;
			}
		case EFUCameraViewType::Roll:
			{
				return bGetMin ? &PCM->ViewRollMin : &PCM->ViewRollMax;
			}
		default:
			return nullptr;
	}
}


FFUCameraViewOverrideEntry::FFUCameraViewOverrideEntry(AFUPlayerCameraManager* PCM, const FFUCameraViewOverride& Override)
{
	AddOverride(PCM, Override);
}

void FFUCameraViewOverrideEntry::AddOverride(AFUPlayerCameraManager* PCM, const FFUCameraViewOverride& Override)
{
	auto& NewOverride = Overrides.Emplace(Override.Type, Override);
	NewOverride.Apply(PCM);
}

void FFUCameraViewOverrideEntry::RemoveOverride(AFUPlayerCameraManager* PCM, EFUCameraViewType Type)
{
	if (auto* Override = Overrides.Find(Type))
	{
		Override->ResetToSnapshot(PCM);
		Overrides.Remove(Type);
	}
}


FFUCameraViewOverrideContainer::FFUCameraViewOverrideContainer() {}

FFUCameraViewOverrideContainer::FFUCameraViewOverrideContainer(AFUPlayerCameraManager* PCM):
	OwnerPCM(PCM)
{}

bool FFUCameraViewOverrideContainer::HasEntry(FName Name) const
{
	return ActiveEntries.Contains(Name);
}

void FFUCameraViewOverrideContainer::AddEntry(FName Name, const FFUCameraViewOverride& Override)
{
	auto* MutableOverride = const_cast<FFUCameraViewOverride*>(&Override);
	MutableOverride->Snapshot.MakeSnapshot(Override.Type, OwnerPCM.Get());
	
	if (auto* Entry = ActiveEntries.Find(Name))
	{
		Entry->AddOverride(OwnerPCM.Get(), Override);
	}
	else if (FU_ENSURE_WEAKVALID_MSG(OwnerPCM, "OwnerPCM is invalid"))
	{
		ActiveEntries.Emplace(Name, FFUCameraViewOverrideEntry(OwnerPCM.Get(), Override));
	}
}

void FFUCameraViewOverrideContainer::RemoveEntry(FName Name, EFUCameraViewType Type)
{
	if (auto* Entry = ActiveEntries.Find(Name))
	{
		Entry->RemoveOverride(OwnerPCM.Get(), Type);

		if (Entry->Overrides.IsEmpty())
		{
			ActiveEntries.Remove(Name);
		}
	}
}

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
AFUPlayerCameraManager::AFUPlayerCameraManager():
	ViewOverrideContainer(this)
{}
