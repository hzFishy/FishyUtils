﻿// By hzFishy 2025 - Do whatever you want with it

#include "Utility/FUValidatorContainer.h"
using namespace FU::Editor::Validator;


FFUEditorValidatorContainerBase::FFUEditorValidatorContainerBase() {}

bool FFUEditorValidatorContainerBase::IsEmpty() const
{
	return Data.IsEmpty();
}

bool FFUEditorValidatorContainerBase::HasMessage() const
{
	return Data.Contains(EFUEditorValidatorStatus::Message);
}
		
bool FFUEditorValidatorContainerBase::HasWarning() const
{
	return Data.Contains(EFUEditorValidatorStatus::Warning);
}

bool FFUEditorValidatorContainerBase::HasError() const
{
	return Data.Contains(EFUEditorValidatorStatus::Error);
}

EFUEditorValidatorStatus FFUEditorValidatorContainerBase::GetHighestStatus() const
{
	if (HasError())
	{
		return EFUEditorValidatorStatus::Error;
	}
	else if (HasWarning())
	{
		return EFUEditorValidatorStatus::Warning;
	}
	else if (HasMessage())
	{
		return EFUEditorValidatorStatus::Message;
	}
	else
	{
		return EFUEditorValidatorStatus::None;
	}
}
