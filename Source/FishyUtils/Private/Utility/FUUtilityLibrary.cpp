// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/FUUtilityLibrary.h"

#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"


FORCEINLINE_DEBUGGABLE
FRotator UFUUtilityLibrary::Add_RotatorRotator(FRotator A, FRotator B)
{
	return A + B;
}

FORCEINLINE_DEBUGGABLE
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
