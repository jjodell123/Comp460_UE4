// Fill out your copyright notice in the Description page of Project Settings.


#include "LockdownGameMode.h"
#include "PlayerCharacter.h"


ALockdownGameMode::ALockdownGameMode ()
{
    DefaultPawnClass = APlayerCharacter::StaticClass();
}