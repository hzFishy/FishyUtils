// By hzFishy 2025 - Do whatever you want with it

#include "FishyUtilsEditor.h"
#include "MessageLogModule.h"
#include "Core/FUEditorSettings.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Utility/FUPickers.h"
#include "Utility/FUPickerSocketCustomization.h"
#include "Utility/FUUtilities.h"
#include "Widgets/Notifications/SNotificationList.h"


#define LOCTEXT_NAMESPACE "FFishyUtilsEditorModule"

void FFishyUtilsEditorModule::StartupModule()
{
	const auto* EditorSettings = GetDefault<UFUEditorSettings>();
	
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FFUPickerSocket::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FFUPickerSocketCustomization::MakeInstance)
	);

	FFUEditor::FFUEditorCommands::Register();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(&FishyEditorInstance, &FFUEditor::RegisterMenuExtensions));
	
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	MessageLogModule.RegisterLogListing("FishyUtils", INVTEXT("Fishy Utils"));
	
#if ENABLE_VISUAL_LOG
	if (EditorSettings->bStopVisualLoggerRecordingOnPIEEnd)
	{
		FWorldDelegates::OnPIEEnded.AddStatic(&FFishyUtilsEditorModule::OnPIEEnded_VisualLogger);
	}
#endif
}

void FFishyUtilsEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout(FFUPickerSocket::StaticStruct()->GetFName());

	FishyEditorInstance.UnregisterMenuExtensions();
	FFUEditor::FFUEditorCommands::Unregister();
	
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	MessageLogModule.UnregisterLogListing("FishyUtils");
}

#if ENABLE_VISUAL_LOG
void FFishyUtilsEditorModule::OnPIEEnded_VisualLogger(UGameInstance* GI)
{
	if (FVisualLogger::Get().IsRecording())
	{
		FVisualLogger::Get().SetIsRecording(false);
		
		FNotificationInfo NotificationInfo(INVTEXT("Visual Logger Stopped"));
		NotificationInfo.ExpireDuration = 2;
		NotificationInfo.Image = FAppStyle::GetBrush("Icons.Info");
		NotificationInfo.SubText = INVTEXT("To disable this see \"Stop VisualLogger Recording On PIE End\" at \"Fishy Utils Editor Settings\"");
		FSlateNotificationManager::Get().AddNotification(NotificationInfo);
	}
}
#endif

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FFishyUtilsEditorModule, FishyUtilsEditor)
