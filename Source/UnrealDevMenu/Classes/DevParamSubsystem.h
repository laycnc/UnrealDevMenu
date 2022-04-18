// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagsManager.h"
class UDevParamType;
class UDevParamDataAsset;
#include "DevParamSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDevMenuParam, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedDevMenuParam,
                                            FGameplayTag,
                                            ParamId);

USTRUCT(BlueprintType)
struct FDevParamStructDummyType
{
	GENERATED_BODY()
};

USTRUCT()
struct FDevParameter
{
	GENERATED_BODY()
public:
	// デバッグパラメータの値の管理
	// 管理方法は後日調整を行う

	UPROPERTY()
	TMap<FName, bool> BoolValues;
	UPROPERTY()
	TMap<FName, int32> Int32Values;
	UPROPERTY()
	TMap<FName, float> FloatValues;
	UPROPERTY()
	TMap<FName, double> DoubleValues;
	UPROPERTY()
	TMap<FName, FName> NameValues;
	UPROPERTY()
	TMap<FName, FString> StringValues;
	UPROPERTY()
	TMap<FName, TWeakObjectPtr<UObject>> ObjectValues;

	// 構造体の特殊化
	TMap<TObjectPtr<UStruct>, TMap<FName, TSharedPtr<FStructOnScope>>>
	    StructValueMap;
};

/**
 * 
 */
UCLASS(MinimalAPI)
class UDevParamSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()


public:
	/**
     * デバッグパラメータサブシステムの取得
     */
	static UNREALDEVMENU_API UDevParamSubsystem* Get(const UObject* InWorldContext);

	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem implementation End

	static void AddReferencedObjects(UObject*             InThis,
	                                 FReferenceCollector& Collector);

	/** 
     * アセットの初期化
     * 
     * @param InAsset デバッグパラメータ記述アセット
     */
	UFUNCTION(BlueprintCallable, Category = "Dev")
	void InitializeAsset(UDevParamDataAsset* InAsset);

	/**
     * bool型のパラメータを設定する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintCallable,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void SetValueByBool(FGameplayTag ParamId, bool NewValue);
	/**
     * 整数型のパラメータを設定する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintCallable,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void SetValueByInt32(FGameplayTag ParamId, int32 NewValue);
	/**
     * 不動点少数型のパラメータを設定する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintCallable,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void SetValueByFloat(FGameplayTag ParamId, float NewValue);
	/**
     * 倍精度不動点少数型のパラメータを設定する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintCallable,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void SetValueByDouble(FGameplayTag ParamId, double NewValue);
	/**
     * Name型のパラメータを設定する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintCallable,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void SetValueByName(FGameplayTag ParamId, FName NewValue);
	/**
     * String型のパラメータを設定する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintCallable,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void SetValueByString(FGameplayTag   ParamId,
	                                        const FString& NewValue);
	/**
     * Object型のパラメータを設定する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintCallable,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void SetValueByObject(FGameplayTag ParamId, UObject* NewValue);

	/**
     * bool型のパラメータを取得する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintPure,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void GetValueByBool(FGameplayTag ParamId,
	                                      bool&        ResultValue) const;
	/**
     * 整数型のパラメータを取得する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintPure,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void GetValueByInt32(FGameplayTag ParamId,
	                                       int32&       ResultValue) const;
	/**
     * 不動点少数型のパラメータを取得する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintPure,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void GetValueByFloat(FGameplayTag ParamId,
	                                       float&       ResultValue) const;
	/**
     * 倍精度不動点少数型のパラメータを取得する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintPure,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void GetValueByDouble(FGameplayTag ParamId,
	                                        double&      ResultValue) const;
	/**
     * Name型のパラメータを取得する
     * 
     * @param ParamId パラメータ名
     * @param NewValue 設定する新規値
     */
	UFUNCTION(BlueprintPure,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void GetValueByName(FGameplayTag ParamId,
	                                      FName&       ResultValue) const;
	/**
     * String型のパラメータを取得する
     * 
     * @param ParamId パラメータ名
     * @param ResultValue 設定する新規値
     */
	UFUNCTION(BlueprintPure,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void GetValueByString(FGameplayTag ParamId,
	                                        FString&     ResultValue) const;
	/**
     * Object型のパラメータを取得する
     * 
     * @param ParamId パラメータ名
     * @param ResultValue 設定する新規値
     */
	UFUNCTION(BlueprintPure,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu|Paramer")
	UNREALDEVMENU_API void GetValueByObject(FGameplayTag ParamId,
	                                        UObject*&    ResultValue) const;

	/**
     * 構造体のパラメータを取得する
     */
	UFUNCTION(BlueprintPure,
	          CustomThunk,
	          meta     = (Categories           = "DevMenuParam",
                      CustomStructureParam = "ResultValue"),
	          Category = "DevMenu|Paramer")
	void GetValueByStructInternal(FGameplayTag              ParamId,
	                              FDevParamStructDummyType& ResultValue) const;

	UNREALDEVMENU_API void GetValueByStruct_Impl(FName          ParamId,
	                                             UScriptStruct* ParamType,
	                                             void*          ResultValue) const;

	template<class T>
	void GetValueByStruct(FName ParamId, T& NewValue) const
	{
		GetValueByStruct_Impl(ParamId, T::StaticStruct(), &NewValue);
	}

	template<class T>
	void GetValueByStruct(FGameplayTag ParamId, T& NewValue) const
	{
		GetValueByStruct_Impl(ParamId.GetTagName(), T::StaticStruct(), &NewValue);
	}

	DECLARE_FUNCTION(execGetValueByStructInternal);

	/**
     * 構造体のパラメータを取得する
     */
	UFUNCTION(BlueprintCallable,
	          CustomThunk,
	          meta     = (Categories           = "DevMenuParam",
                      CustomStructureParam = "NewValue"),
	          Category = "DevMenu|Paramer")
	void SetValueByStructInternal(FGameplayTag                    ParamId,
	                              const FDevParamStructDummyType& NewValue);

	UNREALDEVMENU_API void SetValueByStruct_Impl(FName          ParamId,
	                                             UScriptStruct* ParamType,
	                                             const void*    ResultValue) const;

	template<class T>
	void SetValueByStruct(FName ParamId, const T& NewValue)
	{
		SetValueByStruct_Impl(ParamId, T::StaticStruct(), &NewValue);
	}

	template<class T>
	void SetValueByStruct(FGameplayTag ParamId, const T& NewValue)
	{
		SetValueByStruct_Impl(ParamId.GetTagName(), T::StaticStruct(), &NewValue);
	}

	DECLARE_FUNCTION(execSetValueByStructInternal);

	UNREALDEVMENU_API void RegisterStructParam(FName          ParamId,
	                                           UScriptStruct* ParamType,
	                                           const void*    NewValue);

	template<class T>
	void RegisterStructParam(FName ParamId, const T& NewValue)
	{
		RegisterStructParam(ParamId, T::StaticStruct(), &NewValue);
	}

	template<class T>
	void RegisterStructParam(FGameplayTag ParamId, const T& NewValue)
	{
		RegisterStructParam(ParamId.GetTagName(), T::StaticStruct(), &NewValue);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "DevMenu|Paramer")
	void PrintValue() const;

	/**
     * デバッグパラメータを保存する
     */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "DevMenu|Paramer")
	bool SaveParam(FString SaveFilePath) const;

	/**
     * デバッグパラメータを読み込みする
     */
	UFUNCTION(BlueprintCallable, Category = "DevMenu|Paramer")
	bool LoadParam(FString LoadFilePath);

private:
	template<class T>
	void SetPrimitiveValue(const FGameplayTag& ParamId,
	                       TMap<FName, T>&     TargetValues,
	                       T                   NewValue);

	template<class T>
	void GetPrimitiveValue(const FGameplayTag&   ParamId,
	                       const TMap<FName, T>& TargetValues,
	                       T&                    ResultValue) const;

	template<class T>
	void PrintPrimitiveValue(const TMap<FName, T>& TargetValues) const;

	// 値の追加
	void AddValues(UDevParamType* ParamType);

	// パラメータの終了処理
	void FinalizeDevParam();

private:
	// デバッグパラメータの値の管理
	// 管理方法は後日調整を行う

	UPROPERTY(Transient)
	FDevParameter Parameter;

	UPROPERTY(Transient)
	UDevParamDataAsset* DataAsset = nullptr;

	// 値の変更
	UPROPERTY()
	FOnChangedDevMenuParam OnChangedParam;
};
