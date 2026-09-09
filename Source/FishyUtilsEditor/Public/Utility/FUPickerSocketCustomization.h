// By hzFishy 2025 - Do whatever you want with it

#pragma once

#include "BlueprintComponentReference.h"


class FISHYUTILSEDITOR_API FFUPickerSocketCustomization : public IPropertyTypeCustomization
{
public:
	FFUPickerSocketCustomization();
	
	virtual ~FFUPickerSocketCustomization() override;
	
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
 
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	void RequestSocketRefresh();
	
	TSharedPtr<SComboBox<TSharedPtr<FName>>> SocketPicker;
	TSharedPtr<STextBlock> SocketPickerText;
	TSharedPtr<IPropertyHandle> ComponentReferenceProp;
	TSharedPtr<IPropertyHandle> SocketTypeProp;
	TSharedPtr<IPropertyHandle> ComponentNameProp;
	TSharedPtr<IPropertyHandle> SocketNameProp;

	AActor* OuterActor;
	UClass* OuterActorClass;
	FBlueprintComponentReference ComponentReference;
	TArray<TSharedPtr<FName>> ComponentSockets;
	uint8 SocketFilterFlags;
	FName SelectedSocketName;

	TSharedPtr<FName> GetDefaultSelected() const;
};

