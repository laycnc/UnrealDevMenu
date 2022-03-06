// Copyright Epic Games, Inc. All Rights Reserved.
#include "DevMenuBindingDetails.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "DevMenuBinding.h"
#include "Widgets/Input/SComboBox.h"
#include "DetailLayoutBuilder.h"
#include "Algo/Transform.h"

#define LOCTEXT_NAMESPACE "FDevMenuBindingCustomization"

class SDevMenuBindingFunctionNameWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDevMenuBindingFunctionNameWidget) {}
	SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, LibraryClassProperty)
	SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, FunctionNameProperty)
	SLATE_ARGUMENT(TArray<TWeakObjectPtr<UField>>, FunctionArgsType)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	// メニューを開いた際に呼ばれるイベント
	// 開いたメニューの外観を決定する
	TSharedRef<SWidget> OnGetMenuContent() const;
	// 現在選択されている値
	FText GetCurrentValue() const;
	// 編集可能か？
	bool IsEditEnabled() const;
	// 関数の引数チェック
	bool IsCheckArgs(const UFunction* Function) const;

private:
	TSharedPtr<IPropertyHandle>    LibraryClassProperty;
	TSharedPtr<IPropertyHandle>    FunctionNameProperty;
	TArray<TWeakObjectPtr<UField>> FunctionArgsType;
};

void SDevMenuBindingFunctionNameWidget::Construct(const FArguments& InArgs)
{
	LibraryClassProperty = InArgs._LibraryClassProperty;
	FunctionNameProperty = InArgs._FunctionNameProperty;
	FunctionArgsType     = InArgs._FunctionArgsType;

	// clang-format off
    ChildSlot
    [
        SNew( SComboButton )
        .OnGetMenuContent(this, &SDevMenuBindingFunctionNameWidget::OnGetMenuContent)
        .ContentPadding(FMargin(2.f, 2.f))
        .IsEnabled(this, &SDevMenuBindingFunctionNameWidget::IsEditEnabled)
        .ButtonContent()
        [
            SNew(STextBlock)
            .Text(this, &SDevMenuBindingFunctionNameWidget::GetCurrentValue)
            .Font(IDetailLayoutBuilder::GetDetailFont())
        ]
    ];
	// clang-format on
}

// メニューを開いた際に呼ばれるイベント
// 開いたメニューの外観を決定する
TSharedRef<SWidget> SDevMenuBindingFunctionNameWidget::OnGetMenuContent() const
{
	FMenuBuilder MenuBuilder(true, nullptr);

	UObject* LibraryClassObj = nullptr;
	LibraryClassProperty->GetValue(LibraryClassObj);
	if ( UClass* LibraryClass = Cast<UClass>(LibraryClassObj) )
	{
		if ( LibraryClass->IsChildOf(UBlueprintFunctionLibrary::StaticClass()) )
		{
			for ( UFunction* Func : TFieldRange<UFunction>(LibraryClass) )
			{
				if ( !IsCheckArgs(Func) )
				{
					continue;
				}

				FName     FuncName = Func->GetFName();
				FUIAction Action;
				Action.ExecuteAction = FExecuteAction::CreateLambda(
				    [this, FuncName]()
				    {
					    // 決定時に項目を更新する
					    FunctionNameProperty->SetValue(FuncName);
				    });

				MenuBuilder.AddMenuEntry(FText::FromName(FuncName),
				                         FText::GetEmpty(),
				                         FSlateIcon(),
				                         Action);
			}
		}
	}

	return MenuBuilder.MakeWidget();
}

// 現在選択されている値
FText SDevMenuBindingFunctionNameWidget::GetCurrentValue() const
{
	// 設定されている値を元に指定をする
	FName CurrentValue = NAME_None;
	if ( FunctionNameProperty->GetValue(CurrentValue) == FPropertyAccess::Success )
	{
		return FText::FromName(CurrentValue);
	}
	return FText::GetEmpty();
}

// 編集可能か？
bool SDevMenuBindingFunctionNameWidget::IsEditEnabled() const
{
	return true;
}

// 関数の引数チェック
bool SDevMenuBindingFunctionNameWidget::IsCheckArgs(const UFunction* Function) const
{
	if ( Function->NumParms != FunctionArgsType.Num() )
	{
		// 引数の数が違うのでfalse
		return false;
	}

	if ( !Function->HasAnyFunctionFlags(FUNC_Static) )
	{
		// Static関数以外は除外
		return false;
	}

    // todo: 引数の形をチェックする必要がある
	for ( FProperty* Property : TFieldRange<FProperty>(Function) )
	{

		if ( Property->PropertyFlags & CPF_ReturnParm )
		{
			// 戻り値
		}
		else if ( Property->PropertyFlags & CPF_Parm )
		{
			// 引数
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// FDevMenuBindingCustomization

TSharedRef<IPropertyTypeCustomization> FDevMenuBindingCustomization::MakeInstance()
{
	return MakeShareable(new FDevMenuBindingCustomization());
}

void FDevMenuBindingCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle>      PropertyHandle,
    FDetailWidgetRow&                HeaderRow,
    IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	void* BindingPtr = nullptr;
	if ( PropertyHandle->GetValueData(BindingPtr) == FPropertyAccess::Success )
	{
		if ( const auto* Binding =
		         reinterpret_cast<const FDevMenuBinding*>(BindingPtr) )
		{
			Algo::Transform(
			    Binding->FunctionArgsType,
			    FunctionArgsType,
			    [](TObjectPtr<UField> FunctionArgType) -> TWeakObjectPtr<UField>
			    {
				    return FunctionArgType;
			    });
		}
	}

	// clang-format off
	HeaderRow
        .NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
	    .ValueContent()
        [
            PropertyHandle->CreatePropertyValueWidget()
        ];

	// clang-format on
}

void FDevMenuBindingCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle>      PropertyHandle,
    IDetailChildrenBuilder&          ChildBuilder,
    IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	auto LibraryClassProperty = PropertyHandle->GetChildHandle(
	    GET_MEMBER_NAME_CHECKED(FDevMenuBinding, LibraryClass));
	auto FunctionNameProperty = PropertyHandle->GetChildHandle(
	    GET_MEMBER_NAME_CHECKED(FDevMenuBinding, FunctionName));

	LibraryClassProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(
	    [=]()
	    {
		    //
		    CheckLibraryFunctionName(LibraryClassProperty, FunctionNameProperty);
	    }));

	ChildBuilder.AddProperty(LibraryClassProperty.ToSharedRef());

	// clang-format off
	// クラスが用意されている場合のみNameを表示する
    ChildBuilder
        .AddProperty(FunctionNameProperty.ToSharedRef())
        .CustomWidget()
        .NameContent()
        [
            FunctionNameProperty->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            SNew(SDevMenuBindingFunctionNameWidget)
            	.LibraryClassProperty(LibraryClassProperty)
            	.FunctionNameProperty(FunctionNameProperty)
                .FunctionArgsType(FunctionArgsType)
        ];
	// clang-format on
}

void FDevMenuBindingCustomization::CheckLibraryFunctionName(
    TSharedPtr<IPropertyHandle> LibraryClassProperty,
    TSharedPtr<IPropertyHandle> FunctionNameProperty)
{
	UObject* LibraryClassObj = nullptr;
	if ( FPropertyAccess::Success !=
	     LibraryClassProperty->GetValue(LibraryClassObj) )
	{
		return;
	}
	if ( UClass* LibraryClass = Cast<UClass>(LibraryClassObj) )
	{
		if ( LibraryClass->IsChildOf(UBlueprintFunctionLibrary::StaticClass()) )
		{
			// 関数ライブラリの場合には関数チェックを行う

			FName CurrentValue = NAME_None;
			if ( FPropertyAccess::Success !=
			     FunctionNameProperty->GetValue(CurrentValue) )
			{
				return;
			}

			for ( UFunction* Func : TFieldRange<UFunction>(LibraryClass) )
			{
				if ( Func->GetFName() == CurrentValue )
				{
					// FunctionNameに現在存在する関数無いのメンバが選べれているので何もしない
					return;
				}
			}
			// 関数ライブラリに存在しない関数が選べれているのでNAME_Noneにする
		}
		// ライブラリクラスがUBlueprintFunctionLibraryでは無い場合もNAME_Noneにする
	}
	// ライブラリが未設定の場合にはNAME_Noneにする
	FName NewValue = NAME_None;
	FunctionNameProperty->SetValue(NewValue);
}

#undef LOCTEXT_NAMESPACE
