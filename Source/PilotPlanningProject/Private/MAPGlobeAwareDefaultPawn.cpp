// Copyright 2023 DaduInteractive, Inc. All Rights Reserved.


#include "MAPGlobeAwareDefaultPawn.h"

void AMAPGlobeAwareDefaultPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerCtrl = Cast<APlayerController>(Controller);
	playerCtrl->bShowMouseCursor = true;
	playerCtrl->bEnableClickEvents = true;
	playerCtrl->bEnableMouseOverEvents = true;
}