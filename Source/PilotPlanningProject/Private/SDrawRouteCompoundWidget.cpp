// Copyright 2023 DaduInteractive, Inc. All Rights Reserved.


#include "SDrawRouteCompoundWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SDrawRouteCompoundWidget::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SNew(STextBlock)
            .Text(FText::FromString("Test Button"))
        ]
    + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SNew(SButton)
            .Text(FText::FromString("Press Me"))
        .OnClicked(FOnClicked::CreateSP(this, &SDrawRouteCompoundWidget::OnTestButtonClicked))
        ]
        ]
    + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SNew(STextBlock)
            .Text(FText::FromString("Test Checkbox"))
        ]
    + SHorizontalBox::Slot()
        .VAlign(VAlign_Top)
        [
            SNew(SCheckBox)
            .OnCheckStateChanged(FOnCheckStateChanged::CreateSP(this, &SDrawRouteCompoundWidget::OnTestCheckboxStateChanged))
        //.IsChecked(Fischec::CreateSP(this, &SDrawRouteCompoundWidget::IsTestBoxChecked))
        ]
        ]

        ];
}

FReply SDrawRouteCompoundWidget::OnTestButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Hello. world! The checkbox is %s."), (bIsTestBoxChecked ? TEXT("checked") : TEXT("unchecked")));
    return FReply::Handled();
}
void SDrawRouteCompoundWidget::OnTestCheckboxStateChanged(ECheckBoxState NewState)
{
    bIsTestBoxChecked = NewState == ECheckBoxState::Checked ? true : false;
}
ECheckBoxState SDrawRouteCompoundWidget::IsTestBoxChecked() const
{
    return bIsTestBoxChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

END_SLATE_FUNCTION_BUILD_OPTIMIZATION