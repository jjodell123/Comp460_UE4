// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "MyProjectile.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Disables in built controller rotation
    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    //    FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    ////    FP_MuzzleLocation->SetupAttachment(FP_Gun);
    //    FP_MuzzleLocation->SetRelativeLocation(FVector(42.0f, 40.0f, 40.0f));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();


    // if (GEngine != nullptr)
    // 		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using PlayerCharacter."));

//	if (RootComponent != nullptr) {
//		Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
        // Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
        // Camera->AttachTo(RootComponent);
//	}

    Camera = FindComponentByClass<UCameraComponent>();


}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Handles player movement
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("Strafe", this, &APlayerCharacter::Strafe);
    PlayerInputComponent->BindAxis("Spin", this, &APlayerCharacter::Spin);
    PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::HorizontalRotation);
    // PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::VerticalRotation);
    // PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
    //PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
    //PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("Ascend", this, &APlayerCharacter::Ascend);

    // Bind fire event
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFire);
}

void APlayerCharacter::OnFire()
{
    // Attempt to fire a projectile.
    if (ProjectileClass && !isDead)
    {
        // Get the camera transform.
        FVector ForwardVector = Camera->GetForwardVector();
        FVector CameraLocation = Camera->GetComponentLocation();// GetActorLocation();
        FRotator CameraRotation = Camera->GetComponentRotation();//GetActorRotation();
        // GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // Move the start location to in front of the head
        FVector MuzzleLocation = CameraLocation + ForwardVector * 14;
        FRotator MuzzleRotation = CameraRotation;

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();
            AMyProjectile* Projectile = World->SpawnActor<AMyProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            // Set the projectile's initial trajectory.
            FVector LaunchDirection = MuzzleRotation.Vector();
            Projectile->FireInDirection(LaunchDirection);

            if (!HasAuthority())
            {
                //On the Client
                Server_OnFire(MuzzleLocation, MuzzleRotation);
            }
            else
            {
                // Called from the server
                Multi_OnFire(MuzzleLocation, MuzzleRotation);
            }
        }
    }
}

bool APlayerCharacter::Server_OnFire_Validate(FVector Location, FRotator Rotation)
{
    return true;
}

void APlayerCharacter::Server_OnFire_Implementation(FVector Location, FRotator Rotation)
{
    Multi_OnFire(Location, Rotation);
}

bool APlayerCharacter::Multi_OnFire_Validate(FVector Location, FRotator Rotation)
{
    return true;
}

void APlayerCharacter::Multi_OnFire_Implementation(FVector Location, FRotator Rotation)
{
    if (!IsLocallyControlled()) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        AMyProjectile* Projectile = GetWorld()->SpawnActor<AMyProjectile>(ProjectileClass, Location, Rotation, SpawnParams);
        if (Projectile && !isDead)
        {
            // Set the projectile's initial trajectory.
            FVector LaunchDirection = Rotation.Vector();
            Projectile->FireInDirection(LaunchDirection);
        }
    }
}

// Player moves forwards or backwards
// Val is the movement amount input
void APlayerCharacter::MoveForward(float Val)
{
    if (!isDead) {

        FVector Direction = GetActorForwardVector();
        AddMovementInput(Direction, Val);

        if (!HasAuthority())
        {
            //On the Client
            Server_MoveForward(Val);
        }
        else
        {
            // Called from the server
            Multi_MoveForward(Val);
        }
    }

}

bool APlayerCharacter::Server_MoveForward_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Server_MoveForward_Implementation(float Val)
{
    Multi_MoveForward(Val);
}

bool APlayerCharacter::Multi_MoveForward_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Multi_MoveForward_Implementation(float Val)
{
    if (!IsLocallyControlled()) {
        FVector Direction = GetActorForwardVector();
        AddMovementInput(Direction, Val);
    }
}

// Player moves left or right
void APlayerCharacter::Strafe(float Val)
{
    if (!isDead) {
        // Get right vector
        const FVector Direction = GetActorRightVector();
        AddMovementInput(Direction, Val);

        if (!HasAuthority())
        {
            //On the Client
            Server_Strafe(Val);
        }
        else
        {
            // Called from the server
            Multi_Strafe(Val);
        }
    }

}

bool APlayerCharacter::Server_Strafe_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Server_Strafe_Implementation(float Val)
{
    Multi_Strafe(Val);
}

bool APlayerCharacter::Multi_Strafe_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Multi_Strafe_Implementation(float Val)
{
    if (!IsLocallyControlled()) {
        const FVector Direction = GetActorRightVector();
        AddMovementInput(Direction, Val);
    }
}

void APlayerCharacter::Ascend(float Val)
{
    if (!isDead) {
        FVector Direction = GetActorUpVector();
        AddMovementInput(Direction, Val);

        if (!HasAuthority())
        {
            //On the Client
            Server_Ascend(Val);
        }
        else
        {
            // Called from the server
            Multi_Ascend(Val);
        }
    }

}

bool APlayerCharacter::Server_Ascend_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Server_Ascend_Implementation(float Val)
{
    Multi_Ascend(Val);
}

bool APlayerCharacter::Multi_Ascend_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Multi_Ascend_Implementation(float Val)
{
    if (!IsLocallyControlled()) {
        FVector Direction = GetActorUpVector();
        AddMovementInput(Direction, Val);
    }
}

// Player spins left or right
void APlayerCharacter::Spin(float Val)
{
    if (!isDead) {
        //TODO: check if I can remove one of these
        AddControllerRollInput(Val);
        if (Val)
            AddActorLocalRotation(FQuat(FRotator(0, 0, Val)));

        if (!HasAuthority())
        {
            //On the Client
            Server_Spin(Val);
        }
        else
        {
            // Called from the server
            Multi_Spin(Val);
        }
    }

}

bool APlayerCharacter::Server_Spin_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Server_Spin_Implementation(float Val)
{
    Multi_Spin(Val);
}

bool APlayerCharacter::Multi_Spin_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Multi_Spin_Implementation(float Val)
{
    if (!IsLocallyControlled()) {
        AddControllerRollInput(Val);
        if (Val)
            AddActorLocalRotation(FQuat(FRotator(0, 0, Val)));
    }
}

void APlayerCharacter::HorizontalRotation(float Val)
{
    if (!isDead) {
        UE_LOG(LogTemp, Warning, TEXT("left is %d"), Val);

        // AddControllerYawInput(Val);
        AddActorLocalRotation(FQuat(FRotator(0, Val, 0)));

        // add this later GetWorld()->GetDeltaSeconds()

        if (!HasAuthority())
        {
            //On the Client
            Server_HorizontalRotation(Val);
        }
        else
        {
            // Called from the server
            Multi_HorizontalRotation(Val);
        }
    }

}

bool APlayerCharacter::Server_HorizontalRotation_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Server_HorizontalRotation_Implementation(float Val)
{
    Multi_HorizontalRotation(Val);
}

bool APlayerCharacter::Multi_HorizontalRotation_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Multi_HorizontalRotation_Implementation(float Val)
{
    if (!IsLocallyControlled()) {
        AddActorLocalRotation(FQuat(FRotator(0, Val, 0)));
    }
}


void APlayerCharacter::VerticalRotation(float Val)
{
    if (!isDead) {
        Val *= -1;
        AddActorLocalRotation(FQuat(FRotator(Val, 0, 0)));

        if (!HasAuthority())
        {
            //On the Client
            Server_VerticalRotation(Val);
        }
        else
        {
            // Called from the server
            Multi_VerticalRotation(Val);
        }
    }

}

bool APlayerCharacter::Server_VerticalRotation_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Server_VerticalRotation_Implementation(float Val)
{
    Multi_VerticalRotation(Val);
}

bool APlayerCharacter::Multi_VerticalRotation_Validate(float Val)
{
    return true;
}

void APlayerCharacter::Multi_VerticalRotation_Implementation(float Val)
{
    if (!IsLocallyControlled()) {
        AddActorLocalRotation(FQuat(FRotator(Val, 0, 0)));
    }
}

