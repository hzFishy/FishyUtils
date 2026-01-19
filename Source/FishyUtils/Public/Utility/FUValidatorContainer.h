// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

UENUM(BlueprintType, DisplayName="Fishy Editor Validator Status")
enum class EFUEditorValidatorStatus : uint8
{
	None,
	Message,
	Warning,
	Error
};

namespace FU::Editor::Validator
{
	struct FISHYUTILS_API FFUEditorValidatorContainerBase
	{
		FFUEditorValidatorContainerBase();
		
	protected:
		TMultiMap<EFUEditorValidatorStatus, FString> Data;
		
	public:
		template <typename FmtType, typename... Types>
		void AddMessage(const FmtType& Fmt, Types... Args)
		{
			Data.Emplace(EFUEditorValidatorStatus::Message, FString::Printf(Fmt, Args...));
		}
		
		template <typename FmtType, typename... Types>
		void AddWarning(const FmtType& Fmt, Types... Args)
		{
			Data.Emplace(EFUEditorValidatorStatus::Warning, FString::Printf(Fmt, Args...));
		}
		
		template <typename FmtType, typename... Types>
		void AddError(const FmtType& Fmt, Types... Args)
		{
			Data.Emplace(EFUEditorValidatorStatus::Error, FString::Printf(Fmt, Args...));
		}

		/** Returns true if there is no message, warning or errors */
		bool IsEmpty() const;
		bool HasMessage() const;
		bool HasWarning() const;
		bool HasError() const;
		EFUEditorValidatorStatus GetHighestStatus() const;

		template<typename Func>
		void ForEachEntry(Func InFunc)
		{
			ForEachEntry(EFUEditorValidatorStatus::Error, InFunc);
			ForEachEntry(EFUEditorValidatorStatus::Warning, InFunc);
			ForEachEntry(EFUEditorValidatorStatus::Message, InFunc);
		}
		
		template<typename Func>
		void ForEachEntry(EFUEditorValidatorStatus Status, Func InFunc)
		{
			if (Data.Contains(Status))
			{
				TArray<FString> Values;
				Data.MultiFind(Status, Values, true);

				for (auto& Value : Values)
				{
					InFunc(Status, Value);
				}
			}
		}
	};
	
}
