// Copyright Epic Games, Inc. All Rights Reserved.
#include "DevParamStructTypeDetails.h"
#include "ParamType/DevParamStructType.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"

TSharedRef<IDetailCustomization> FDevParamStructTypeCustomization::MakeInstance()
{
	return MakeShareable(new FDevParamStructTypeCustomization());
}

void FDevParamStructTypeCustomization::CustomizeDetails(
    IDetailLayoutBuilder& DetailBuilder)
{

	auto Selected = DetailBuilder.GetSelectedObjects();

	if ( Selected.Num() != 1 )
	{
		return;
	}

	IDetailCategoryBuilder& DevMenuCategoryBuilder =
	    DetailBuilder.EditCategory("DevMenu");

	UDevParamStructType* StructType = Cast<UDevParamStructType>(Selected[0].Get());

	if ( !IsValid(StructType) )
	{
		return;
	}

	if ( StructType->GetDefaultValue().IsValid() )
	{
		// 構造体を表示させる
		IDetailPropertyRow* DefaultValueRow =
		    DevMenuCategoryBuilder.AddExternalStructure(
		        StructType->GetDefaultValue());

		if ( DefaultValueRow )
		{
			TSharedPtr<IPropertyHandle> Property =
			    DefaultValueRow->GetPropertyHandle();

			if ( Property.IsValid() )
			{
				const auto OnPropertyValueChanged = FSimpleDelegate::CreateLambda(
				    [StructType]()
				    {
					    // 構造体が変更されたのでOuterに対して変更フラグを立てる必要がある
					    StructType->Modify();
				    });

				// 変更イベントに登録する
				Property->SetOnPropertyValueChanged(OnPropertyValueChanged);
				Property->SetOnChildPropertyValueChanged(OnPropertyValueChanged);
			}
		}
	}
}
