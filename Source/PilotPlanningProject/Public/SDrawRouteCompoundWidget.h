 // Copyright 2023 DaduInteractive, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class PILOTPLANNINGPROJECT_API SDrawRouteCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDrawRouteCompoundWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
public:
	FReply OnTestButtonClicked();
	void OnTestCheckboxStateChanged(ECheckBoxState NewState);
	ECheckBoxState IsTestBoxChecked() const;

protected:
	bool bIsTestBoxChecked;
};
