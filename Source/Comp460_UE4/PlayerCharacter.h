// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "MyProjectile.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USceneComponent;
class UAnimMontage;
class USoundBase;

UCLASS()
class COMP460_UE4_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
    
//    /** Location on gun mesh where projectiles should spawn. */
//    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
//    USceneComponent* FP_MuzzleLocation;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    // Projectile class to spawn.
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AMyProjectile> ProjectileClass;

public:
    
    /** Gun muzzle's offset from the characters location */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
    FVector MuzzleOffset;


    /** Sound to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
    USoundBase* FireSound;

    /** AnimMontage to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    UAnimMontage* FireAnimation;
    
    
    //Function handling firing projectiles
    UFUNCTION()
    void OnFire();
    
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_OnFire(FVector Location, FRotator Rotation);
    bool Server_OnFire_Validate(FVector Location, FRotator Rotation);
    void Server_OnFire_Implementation(FVector Location, FRotator Rotation);
    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//protected:
//        /** Fires a projectile. */
//    void OnFire();

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
