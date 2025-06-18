#include "UpgradeNodeWidget.h"
#include "Components/TextBlock.h"

void UUpgradeNodeWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    // Update the name and description text blocks if they are valid.
    if (Text_Name)
    {
        Text_Name->SetText(UpgradeName);
    }
    if (Text_Description)
    {
        Text_Description->SetText(UpgradeDescription);
    }

    // Update the common cost text and visibility.
    if (Text_CommonCost)
    {
        FString CostString = FString::Printf(TEXT("Mechanical: %d"), CommonCost);
        Text_CommonCost->SetText(FText::FromString(CostString));

        // Set visibility based on if the cost is greater than zero.
        ESlateVisibility NewVisibility = (CommonCost > 0) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
        Text_CommonCost->SetVisibility(NewVisibility);
    }

    // Update the uncommon cost text and visibility.
    if (Text_UncommonCost)
    {
        FString CostString = FString::Printf(TEXT("Electronic: %d"), UncommonCost);
        Text_UncommonCost->SetText(FText::FromString(CostString));

        // Set visibility based on if the cost is greater than zero.
        ESlateVisibility NewVisibility = (UncommonCost > 0) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
        Text_UncommonCost->SetVisibility(NewVisibility);
    }

    // Update the rare cost text and visibility.
    if (Text_RareCost)
    {
        FString CostString = FString::Printf(TEXT("Robotic: %d"), RareCost);
        Text_RareCost->SetText(FText::FromString(CostString));

        // Set visibility based on if the cost is greater than zero.
        ESlateVisibility NewVisibility = (RareCost > 0) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
        Text_RareCost->SetVisibility(NewVisibility);
    }
}