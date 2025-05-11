// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/FUPickers.h"

EFUEComponentSocketType FU_Pickers::ConvertSocketTypeFromEngine(EComponentSocketType::Type Type)
{
	return static_cast<EFUEComponentSocketType>(Type);
}

FFUPickerSockets::FFUPickerSockets()
{
	
}