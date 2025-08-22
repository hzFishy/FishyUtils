// By hzFishy 2025 - Do whatever you want with it


#include "Utility/FUPickers.h"

EFUEComponentSocketType FU_Pickers::ConvertSocketTypeFromEngine(EComponentSocketType::Type Type)
{
	return static_cast<EFUEComponentSocketType>(Type);
}

FFUPickerSocket::FFUPickerSocket()
{
	
}
