// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "FUVisualLogger.generated.h"


UCLASS(DefaultConfig, Config="Game", DisplayName="Fishy Utils Visual Logger")
class FISHYUTILS_API UFUVisualLoggerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UFUVisualLoggerSettings* Get();

	/** NOT IMPLEMENTED */
	UPROPERTY(Config, EditAnywhere, Category="Categories")
	TArray<FString> CategoriesToIgnore;
	
	virtual FName GetCategoryName() const override;
};


#if ENABLE_VISUAL_LOG
class FISHYUTILS_API FFUVisualLoggerManager : public TSharedFromThis<FFUVisualLoggerManager>
{
protected:
	static TSharedPtr<FFUVisualLoggerManager> Instance;
	FDelegateHandle OnFilterCategoryAddedHandle;
	
public:
	static void Initialize();
	
	static void Deinitialize();
	
	void OnFilterCategoryAdded(FString Name, ELogVerbosity::Type Verbosity);
};
#endif
