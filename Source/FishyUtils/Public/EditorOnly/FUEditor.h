// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_EDITOR

#include "Framework/Commands/Commands.h"


class FFUEditor
{
public:
	class FFUEditorCommands : public TCommands<FFUEditorCommands>
	{
	public:
		FFUEditorCommands() : TCommands<FFUEditorCommands>(
				"FishyEditorCommands",
				FText::FromString("Fishy Utilities"),
				NAME_None,
				FAppStyle::GetAppStyleSetName()
			)
		{}

		TSharedPtr<FUICommandInfo> SelectSameFolderLevel;

		virtual void RegisterCommands() override;
	};
	
public:
	FFUEditor();
	~FFUEditor();
	
	void RegisterMenuExtensions();
	
	void UnregisterMenuExtensions();

protected:
	void ExecuteSelectSameFolderLevel();
};

#endif
