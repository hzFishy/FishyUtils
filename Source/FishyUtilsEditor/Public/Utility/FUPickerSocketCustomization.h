// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "BlueprintComponentReference.h"


class FISHYUTILSEDITOR_API FFUPickerSocketCustomization : public IPropertyTypeCustomization
{
public:
	virtual ~FFUPickerSocketCustomization() override;
	
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
 
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	void RequestSocketRefresh();
	
	TSharedPtr<SComboBox<TSharedPtr<FName>>> SocketPicker = nullptr;
	TSharedPtr<STextBlock> SocketPickerText = nullptr;
	TSharedPtr<IPropertyHandle> ComponentReferenceProp = nullptr;
	TSharedPtr<IPropertyHandle> SocketTypeProp = nullptr;
	TSharedPtr<IPropertyHandle> ComponentNameProp = nullptr;
	TSharedPtr<IPropertyHandle> SocketNameProp = nullptr;

	AActor* OuterActor = nullptr;
	UClass* OuterActorClass = nullptr;
	FBlueprintComponentReference ComponentReference;
	TArray<TSharedPtr<FName>> ComponentSockets;
	uint8 SocketFilterFlags = 0;
	FName SelectedSocketName;

	TSharedPtr<FName> GetDefaultSelected() const;
};

