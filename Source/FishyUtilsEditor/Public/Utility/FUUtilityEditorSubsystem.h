// Copyright Brutal Sun Dev Team. All Rights Reserved.

#pragma once

#include "EditorSubsystem.h"
#include "FUUtilityEditorSubsystem.generated.h"


UCLASS()
class FISHYUTILSEDITOR_API UFUUtilityEditorSubsystem : public UEditorSubsystem, public FSelfRegisteringExec
{
	GENERATED_BODY()
	
	enum EFUMirrorType
	{
		None,
		X,
		Y,
		Z
	};
	
	struct FFUMirrorEntry
	{
		FFUMirrorEntry(AActor* InActor);
		
		TWeakObjectPtr<AActor> Actor;
		FTransform OriginalTransform;
	};
	
	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
protected:
	FTSTicker::FDelegateHandle TickHandle;
	
	EFUMirrorType HandledMirrorType;
	TArray<FFUMirrorEntry> HandledMirrorEntries;
	TSharedPtr<SWidget> LastTickHoveredWidget;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	virtual bool Exec_Editor(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	
	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
	bool Tick(float DeltaTime);
	
	
	/*----------------------------------------------------------------------------
		Mirror
	----------------------------------------------------------------------------*/
protected:
	void StartMirrorType(EFUMirrorType Type);
	
	void StopCurrentMirrorType(bool bRestore);
};
