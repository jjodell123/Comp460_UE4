// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "MyProjectile.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;


    /** Sound to play each time we fire */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
        class USoundCue* FireSoundCue;

    /** Sound to play each time we fire */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
        class USoundCue* JetPackSoundCue;

    /** AnimMontage to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    UAnimMontage* FireAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
    bool isDead = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
    int Damage = 25;

    // Movement speeds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
    float MouseSensitivity = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
    float AscendSpeed = 10.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
    float StrafeSpeed = 10.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
    float ForwardSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
    float SpinSpeed = 10.0f;




    //Function handling firing projectiles
    UFUNCTION()
        void OnFire();

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_OnFire(FVector Location, FRotator Rotation);
    bool Server_OnFire_Validate(FVector Location, FRotator Rotation);
    void Server_OnFire_Implementation(FVector Location, FRotator Rotation);

    UFUNCTION(NetMulticast, Reliable, WithValidation)
    void Multi_OnFire(FVector Location, FRotator Rotation);
    bool Multi_OnFire_Validate(FVector Location, FRotator Rotation);
    void Multi_OnFire_Implementation(FVector Location, FRotator Rotation);

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

    //Audio Component
    UAudioComponent* FireSoundAudioComponent;
    UAudioComponent* JetPackSoundAudioComponent;

    // Handles forward/backward movement input
    UFUNCTION()
    void MoveForward(float Val);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_MoveForward(float Val);
    bool Server_MoveForward_Validate(float Val);
    void Server_MoveForward_Implementation(float Val);

    UFUNCTION(NetMulticast, Reliable, WithValidation)
    void Multi_MoveForward(float Val);
    bool Multi_MoveForward_Validate(float Val);
    void Multi_MoveForward_Implementation(float Val);

    UFUNCTION()
    void Strafe(float Val);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Strafe(float Val);
    bool Server_Strafe_Validate(float Val);
    void Server_Strafe_Implementation(float Val);

    UFUNCTION(NetMulticast, Reliable, WithValidation)
    void Multi_Strafe(float Val);
    bool Multi_Strafe_Validate(float Val);
    void Multi_Strafe_Implementation(float Val);

    UFUNCTION()
    void Ascend(float Val);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Ascend(float Val);
    bool Server_Ascend_Validate(float Val);
    void Server_Ascend_Implementation(float Val);

    UFUNCTION(NetMulticast, Reliable, WithValidation)
    void Multi_Ascend(float Val);
    bool Multi_Ascend_Validate(float Val);
    void Multi_Ascend_Implementation(float Val);

    UFUNCTION()
    void Spin(float Val);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Spin(float Val);
    bool Server_Spin_Validate(float Val);
    void Server_Spin_Implementation(float Val);

    UFUNCTION(NetMulticast, Reliable, WithValidation)
    void Multi_Spin(float Val);
    bool Multi_Spin_Validate(float Val);
    void Multi_Spin_Implementation(float Val);

    UFUNCTION()
    void HorizontalRotation(float Val);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_HorizontalRotation(float Val);
    bool Server_HorizontalRotation_Validate(float Val);
    void Server_HorizontalRotation_Implementation(float Val);

    UFUNCTION(NetMulticast, Reliable, WithValidation)
    void Multi_HorizontalRotation(float Val);
    bool Multi_HorizontalRotation_Validate(float Val);
    void Multi_HorizontalRotation_Implementation(float Val);

    UFUNCTION()
    void VerticalRotation(float Val);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_VerticalRotation(float Val);
    bool Server_VerticalRotation_Validate(float Val);
    void Server_VerticalRotation_Implementation(float Val);

    UFUNCTION(NetMulticast, Reliable, WithValidation)
    void Multi_VerticalRotation(float Val);
    bool Multi_VerticalRotation_Validate(float Val);
    void Multi_VerticalRotation_Implementation(float Val);

    UPROPERTY()
    UCameraComponent* Camera;
};