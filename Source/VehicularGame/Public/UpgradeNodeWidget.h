#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeNodeWidget.generated.h"

class UTextBlock;

UCLASS()
class VEHICULARGAME_API UUpgradeNodeWidget : public UUserWidget
{
    GENERATED_BODY()
    
protected:
    virtual void NativePreConstruct() override;

    // --- DATA VARIABLES ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade Node Data")
    FText UpgradeName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade Node Data")
    FText UpgradeDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade Node Data")
    int32 CommonCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade Node Data")
    int32 UncommonCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade Node Data")
    int32 RareCost;

    // --- WIDGET BINDINGS ---
    // The variable names here MUST match the widget names in your WBP_UpgradeNode.

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Name;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Description;

    // We now have three separate bindings for each cost text block.
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_CommonCost;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_UncommonCost;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_RareCost;
};