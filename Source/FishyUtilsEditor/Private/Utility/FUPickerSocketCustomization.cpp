// By hzFishy 2025 - Do whatever you want with it


#include "Utility/FUPickerSocketCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "Utility/FUPickers.h"


FFUPickerSocketCustomization::~FFUPickerSocketCustomization()
{
	SocketPicker.Reset();
	SocketPickerText.Reset();
	ComponentReferenceProp.Reset();
	SocketTypeProp.Reset();
	ComponentNameProp.Reset();
	SocketNameProp.Reset();
	ComponentSockets.Empty();
}

TSharedRef<IPropertyTypeCustomization> FFUPickerSocketCustomization::MakeInstance()
{
	return MakeShared<FFUPickerSocketCustomization>();
}

void FFUPickerSocketCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	TArray<UObject*> ObjectList;
	PropertyHandle->GetOuterObjects(ObjectList);

	// Handle common cases:
	// - blueprint of Actor
	// - instance of Actor
	// - instance of ActorComponent
	for (UObject* OuterObject : ObjectList)
	{
		while (IsValid(OuterObject))
		{
			if (AActor* Actor = Cast<AActor>(OuterObject))
			{
				OuterActor = Actor;
				break;
			}
			if (UActorComponent* Component = Cast<UActorComponent>(OuterObject))
			{
				if (Component->GetOwner())
				{
					OuterActor = Component->GetOwner();
					break;
				}
			}
			// only support regular blueprints (not anim or others)
			if (UBlueprintGeneratedClass* Class = ExactCast<UBlueprintGeneratedClass>(OuterObject))
			{
				OuterActorClass = Class;
				break;
			}
			OuterObject = OuterObject->GetOuter();
		}
	}

	// DetermineContext_FromFunctionProperty
	// handle case when BCR is a local variable in a function declared in blueprint of AActor
	if (!OuterActor && !OuterActorClass && !ObjectList.Num())
	{
		FProperty* Property = PropertyHandle->GetProperty();
		UFunction* OwnerFunction = Property && !Property->IsNative() ? Property->GetOwner<UFunction>() : nullptr;
		if (OwnerFunction && OwnerFunction->GetOwnerClass())
		{
			UClass* const OwnerClass = OwnerFunction->GetOwnerClass();
			if (ExactCast<UBlueprint>(OwnerClass->ClassGeneratedBy) && OwnerClass->IsChildOf(AActor::StaticClass()))
			{
				OuterActorClass = OwnerClass;
			}
		}
	}

	if (!OuterActor && OuterActorClass)
	{
		OuterActor = OuterActorClass->GetDefaultObject<AActor>();
	}

	if (OuterActor && !OuterActorClass)
	{
		OuterActorClass = OuterActor->GetClass();
	}
}

void FFUPickerSocketCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	SocketTypeProp = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FFUPickerSocket, SocketFilterFlags));
	SocketTypeProp->GetValue(SocketFilterFlags);
	SocketTypeProp->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([this]()
	{
		SocketTypeProp->GetValue(SocketFilterFlags);
		RequestSocketRefresh();
	}));
	
	ComponentReferenceProp = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FFUPickerSocket, ComponentReference));
	void* OutAddress = nullptr;
	ComponentReferenceProp->GetValueData(OutAddress);
	ComponentReference = *static_cast<FBlueprintComponentReference*>(OutAddress);
	ComponentReferenceProp->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([this]()
	{
		void* OutAddress = nullptr;
		ComponentReferenceProp->GetValueData(OutAddress);
		ComponentReference = *static_cast<FBlueprintComponentReference*>(OutAddress);
		RequestSocketRefresh();
	}));

	auto& SocketSelectionGroup = ChildBuilder.AddGroup(
		FName("Socket Selection"),
		FText::FromString(FString::Printf(TEXT("%s (Socket Selection)"), *PropertyHandle->GetProperty()->GetName()))
	);
	
	SocketNameProp = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FFUPickerSocket, SelectedSocket));
	SocketNameProp->GetValue(SelectedSocketName);
	auto& SocketGroup = ChildBuilder.AddGroup(
		FName("Socket Value"),
		FText::FromString(FString::Printf(TEXT("%s (Socket Value)"), *PropertyHandle->GetProperty()->GetName()))
	);
	SocketGroup.AddPropertyRow(SocketNameProp.ToSharedRef());
	SocketGroup.ToggleExpansion(true);

	SocketSelectionGroup.AddPropertyRow(ComponentReferenceProp.ToSharedRef());
	SocketSelectionGroup.AddPropertyRow(SocketTypeProp.ToSharedRef());
	

	
	RequestSocketRefresh();

	IDetailPropertyRow& DynamicSelectedSocketNamePropertyRow = SocketSelectionGroup.AddPropertyRow(SocketNameProp.ToSharedRef());
	
	DynamicSelectedSocketNamePropertyRow.CustomWidget()
		.NameContent()
		[
			SNew(STextBlock)
				.Text(FText::FromString("Browser"))
				.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent()
		[
			SAssignNew(SocketPicker, SComboBox<TSharedPtr<FName>>)
				.OptionsSource(&ComponentSockets)
				.InitiallySelectedItem(GetDefaultSelected())
				.Content()
				[
					SAssignNew(SocketPickerText, STextBlock)
						.Text(FText::FromString(GetDefaultSelected()->ToString()))
				]
				.OnGenerateWidget_Lambda([this](TSharedPtr<FName> InItem)
				{
					return SNew(STextBlock).Text(FText::FromString(InItem.Get()->ToString()));
				})
				.ToolTipText(FText::FromString("Select a socket from the given component (can be empty)"))
				.OnSelectionChanged_Lambda([this](TSharedPtr<FName> Arg, ESelectInfo::Type Type)
				{
					if (SocketNameProp.IsValid() && Arg.IsValid())
					{
						if (Type != ESelectInfo::Type::Direct)
						{
							SocketNameProp->SetValue(*Arg.Get());
							SocketPicker->SetSelectedItem(MakeShared<FName>(SelectedSocketName.IsValid() ? SelectedSocketName : "None"));
						}
					}
				})
		];
}

void FFUPickerSocketCustomization::RequestSocketRefresh()
{
	if (IsValid(OuterActor) && IsValid(OuterActorClass) && !ComponentReference.IsNull())
	{
		const UPrimitiveComponent* TargetComponent = AActor::GetActorClassDefaultComponentByName<UPrimitiveComponent>(OuterActorClass, ComponentReference.GetValue());

		if (IsValid(TargetComponent))
		{
			ComponentSockets.Empty();
			if (TargetComponent->HasAnySockets() && SocketFilterFlags > 0)
			{
				TArray<FComponentSocketDescription> SocketsDescription;
				TargetComponent->QuerySupportedSockets(SocketsDescription);

				for (auto& SocketDescription : SocketsDescription)
				{
					EFUEComponentSocketType Type = FU_Pickers::ConvertSocketTypeFromEngine(SocketDescription.Type);
					if ((SocketFilterFlags & (uint8)Type) == (uint8)Type)
					{
						ComponentSockets.Add(MakeShared<FName>(SocketDescription.Name));
					}
				}
			}
		}
	}

	if (SocketPicker.IsValid())
	{
		SocketPicker->RefreshOptions();
		if (!SelectedSocketName.IsValid())
		{
			SocketPicker->SetSelectedItem(GetDefaultSelected());
		}

		if (ComponentReference.IsNull())
		{
			SocketNameProp->SetValue(NAME_None);
		}

		SocketPickerText->SetText(FText::FromString(GetDefaultSelected()->ToString()));
	}
}

TSharedPtr<FName> FFUPickerSocketCustomization::GetDefaultSelected() const
{
	return MakeShared<FName>(ComponentReference.IsNull() ? "No component selected" : ComponentSockets.IsEmpty() ? "Nothing found" : "Click to browse");
}
