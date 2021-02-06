// Copyright Epic Games, Inc. All Rights Reserved.


#include "Comp460_UE4GameModeBase.h"
#include "PlayerCharacter.h"

AComp460_UE4GameModeBase::AComp460_UE4GameModeBase ()
{
    DefaultPawnClass = APlayerCharacter::StaticClass();
}