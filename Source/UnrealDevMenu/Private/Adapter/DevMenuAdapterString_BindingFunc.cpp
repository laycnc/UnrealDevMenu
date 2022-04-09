// Fill out your copyright notice in the Description page of Project Settings.

#include "Adapter/DevMenuAdapterString_BindingFunc.h"
#include "DevMenuSubsystem.h"

UDevMenuAdapterString_BindingFunc::UDevMenuAdapterString_BindingFunc(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	Setter.FunctionArgsType.Add(TEXT("FString"));
	Setter.FunctionArgsType.Add(TEXT("UObject*"));

	Getter.FunctionArgsType.Add(TEXT("UObject*"));
	Getter.FunctionArgsType.Add(TEXT("FString"));
}

// 値が設定された
void UDevMenuAdapterString_BindingFunc::SetValue_Implementation(
    UDevMenuSubsystem* InSubsystem,
    const FString&     NewValue) const
{
	struct FArgs
	{
		FString        NewValue;
		const UObject* WorldContext;
	};

	FArgs Args        = {};
	Args.WorldContext = InSubsystem;
	Args.NewValue     = NewValue;

	// 関数経由で実行
	Setter.Invoke(&Args);
}

// 値の取得
FString UDevMenuAdapterString_BindingFunc::GetValue_Implementation(
    UDevMenuSubsystem* InSubsystem) const
{

	struct FArgs
	{
		const UObject* WorldContext;
		FString        Result;
	};

	FArgs Args        = {};
	Args.WorldContext = InSubsystem;

	// イベントの実行
	Getter.Invoke(&Args);

	return Args.Result;
}
