// By hzFishy - 2026 - Do whatever you want with it.


#include "Utility/FUUtilityLibrary.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/FUUtilities.h"


FIntVector2 UFUUtilityLibrary::Add_IntVector2IntVector2(FIntVector2 A, FIntVector2 B)
{
	return A + B;
}

FIntVector2 UFUUtilityLibrary::Substract_IntVector2IntVector2(FIntVector2 A, FIntVector2 B)
{
	return A - B;
}

FIntVector2 UFUUtilityLibrary::Multiply_IntVector2IntVector2(FIntVector2 A, FIntVector2 B)
{
	return A * B;
}

FIntVector2 UFUUtilityLibrary::Divide_IntVector2IntVector2(FIntVector2 A, FIntVector2 B)
{
	return FIntVector2(A.X / B.X, A.Y / B.Y);
}

bool UFUUtilityLibrary::Equal_IntVector2IntVector2(FIntVector2 A, FIntVector2 B)
{
	return A == B;
}

bool UFUUtilityLibrary::NotEqual_IntVector2IntVector2(FIntVector2 A, FIntVector2 B)
{
	return A != B;
}

FIntVector UFUUtilityLibrary::Add_IntVectorIntVector(FIntVector A, FIntVector B)
{
	return A + B;
}

FIntVector UFUUtilityLibrary::Substract_IntVectorIntVector(FIntVector A, FIntVector B)
{
	return A - B;
}

FIntVector UFUUtilityLibrary::Multiply_IntVectorIntVector(FIntVector A, FIntVector B)
{
	return A * B;
}

FIntVector UFUUtilityLibrary::Divide_IntVectorIntVector(FIntVector A, FIntVector B)
{
	return FIntVector(A.X / B.X, A.Y / B.Y, A.Z / B.Z);
}

bool UFUUtilityLibrary::Equal_IntVectorIntVector(FIntVector A, FIntVector B)
{
	return A == B;
}

bool UFUUtilityLibrary::NotEqual_IntVectorIntVector(FIntVector A, FIntVector B)
{
	return A != B;
}

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

void UFUUtilityLibrary::StringArrayToString(const TArray<FString>& Array, FString& OutString, const FString& Separator, const FString& Prefix, const FString& Suffix)
{
	FU::Utils::ArrayToString(Array, OutString, Separator, Prefix, Suffix);
}

void UFUUtilityLibrary::IntVectorArrayToString(const TArray<FIntVector>& Array, FString& OutString, const FString& Separator, const FString& Prefix, const FString& Suffix)
{
	FU::Utils::ArrayToStringTemplate<FIntVector>(Array, [] (FIntVector Item) { return Item.ToString(); }, OutString, Separator, Prefix, Suffix);
}

void UFUUtilityLibrary::IntVector2ArrayToString(const TArray<FIntVector2>& Array, FString& OutString, const FString& Separator, const FString& Prefix, const FString& Suffix)
{
	FU::Utils::ArrayToStringTemplate<FIntVector2>(Array, [] (FIntVector2 Item) { return Item.ToString(); }, OutString, Separator, Prefix, Suffix);
}

void UFUUtilityLibrary::VectorArrayToString(const TArray<FVector>& Array, FString& OutString, const FString& Separator, const FString& Prefix, const FString& Suffix)
{
	FU::Utils::ArrayToStringTemplate<FVector>(Array, [] (FVector Item) { return FU::Utils::PrintCompactVector(Item); }, OutString, Separator, Prefix, Suffix);
}

void UFUUtilityLibrary::RotatorArrayToString(const TArray<FRotator>& Array, FString& OutString, const FString& Separator, const FString& Prefix, const FString& Suffix)
{
	FU::Utils::ArrayToStringTemplate<FRotator>(Array, [] (FRotator Item) { return FU::Utils::PrintCompactRotator(Item); }, OutString, Separator, Prefix, Suffix);
}

void UFUUtilityLibrary::TransformArrayToString(const TArray<FTransform>& Array, FString& OutString, const FString& Separator, const FString& Prefix, const FString& Suffix)
{
	FU::Utils::ArrayToStringTemplateRef<FTransform>(Array, [] (const FTransform& Item) { return Item.ToString(); }, OutString, Separator, Prefix, Suffix);
}
