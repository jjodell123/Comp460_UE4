// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class COMP460_UE4_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Handles forward/backward movement input
	UFUNCTION()
	void MoveForward(float Val);

	UFUNCTION()
	void Strafe(float Val);

	UFUNCTION()
	void Ascend(float Val);

	UFUNCTION()
	void Spin(float Val);

	UFUNCTION()
	void HorizontalRotation(float Val);

	UFUNCTION()
	void VerticalRotation(float Val);

	UPROPERTY()
	UCameraComponent* Camera;

};
