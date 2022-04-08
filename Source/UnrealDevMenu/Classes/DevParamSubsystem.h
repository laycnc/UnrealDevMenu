// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagsManager.h"
class UDevParamType;
class UDevParamDataAsset;
#include "DevParamSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDevMenuParam, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedDevMenuParam,
                                            FGameplayTag,
                                            ParamId);

/**
 * 
 */
UCLASS(MinimalAPI)
class UDevParamSubsystem : public UGameInstanceSubsystem
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

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "DevMenu|Paramer")
	void PrintValue() const;

private:
	template<class T>
	void SetPrimitiveValue(const FGameplayTag&    ParamId,
	                       TMap<FGameplayTag, T>& TargetValues,
	                       T                      NewValue);

	template<class T>
	void GetPrimitiveValue(const FGameplayTag&          ParamId,
	                       const TMap<FGameplayTag, T>& TargetValues,
	                       T&                           ResultValue) const;

	template<class T>
	void PrintPrimitiveValue(const TMap<FGameplayTag, T>& TargetValues) const;

	// 値の追加
	void AddValues(UDevParamType* ParamType);


private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, bool> BoolValues;
	UPROPERTY(Transient)
	TMap<FGameplayTag, int32> Int32Values;
	UPROPERTY(Transient)
	TMap<FGameplayTag, float> FloatValues;
	UPROPERTY(Transient)
	TMap<FGameplayTag, double> DoubleValues;
	UPROPERTY(Transient)
	TMap<FGameplayTag, FName> NameValues;
	UPROPERTY(Transient)
	TMap<FGameplayTag, FString> StringValues;
	UPROPERTY(Transient)
	TMap<FGameplayTag, TWeakObjectPtr<UObject>> ObjectValues;

	// 構造体の特殊化
	TMap<TObjectPtr<UStruct>, TMap<FGameplayTag, FStructOnScope>> StructValues;

	UPROPERTY(Transient)
	UDevParamDataAsset* DataAsset = nullptr;

	// 値の変更
	UPROPERTY()
	FOnChangedDevMenuParam OnChangedParam;
};
