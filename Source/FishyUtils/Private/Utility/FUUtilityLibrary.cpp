// By hzFishy 2025 - Do whatever you want with it


#include "Utility/FUUtilityLibrary.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"


FRotator UFUUtilityLibrary::Add_RotatorRotator(FRotator A, FRotator B)
{
	return A + B;
}

FRotator UFUUtilityLibrary::Subtract_RotatorRotator(FRotator A, FRotator B)
{
	return A - B;
}

bool UFUUtilityLibrary::AsyncOpenLevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level, bool bAbsolute, FString Options)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) { return false; }
	if (Level.IsNull()) { return false; }

	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!AssetManager) { return false; }

	AssetManager->GetStreamableManager().RequestAsyncLoad(Level.ToSoftObjectPath(), [WorldContextObject, Level, bAbsolute, Options] ()
	{
		if (!Level.IsValid()) { return; }

		UGameplayStatics::OpenLevelBySoftObjectPtr(WorldContextObject, Level, bAbsolute, Options);
	});
	
	return true;
}

float UFUUtilityLibrary::FindDeltaAngleDegrees(float A, float B)
{
	return FMath::FindDeltaAngleDegrees(A, B);
}
