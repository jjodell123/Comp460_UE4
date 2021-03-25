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
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
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
	// PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::HorizontalRotation);
	// PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	// // PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::VerticalRotation);
	// PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Ascend", this, &APlayerCharacter::Ascend);
    
    // Bind fire event
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFire);
}

void APlayerCharacter::OnFire()
{
//    UE_LOG(LogTemp, Warning, TEXT("FIRING"));
        // Attempt to fire a projectile.
    if (ProjectileClass)
    {
        // Get the camera transform.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // Set MuzzleOffset to spawn projectiles slightly in front of the camera.
//        MuzzleOffset.Set(0.01f, 0.0f, 0.0f);

        // Transform MuzzleOffset from camera space to world space.
        FVector MuzzleLocation = CameraLocation + (-50.0f, -25.0f, -50.0f);
        
//        FVector MuzzleLocation = CameraLocation;
        
        // Skew the aim to be slightly upwards.
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 10.0f;

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            
            
            if (!HasAuthority())
            {
                //On the Client
                Server_OnFire(MuzzleLocation, MuzzleRotation);
            }
            else
            {
                //On the Server
                // Spawn the projectile at the muzzle.
                AMyProjectile* Projectile = World->SpawnActor<AMyProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
                if (Projectile)
                {
                    // Set the projectile's initial trajectory.
                    FVector LaunchDirection = MuzzleRotation.Vector();
                    Projectile->FireInDirection(LaunchDirection);
                }
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
    //Since this is done on the server, All the clients should see this happen as well.
    
    UE_LOG(LogTemp, Warning, TEXT("Server OnFire Implementation has been Called!"));
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    AMyProjectile* Projectile = GetWorld()->SpawnActor<AMyProjectile>(ProjectileClass, Location, Rotation, SpawnParams);
    if (Projectile)
        {
            // Set the projectile's initial trajectory.
            FVector LaunchDirection = Rotation.Vector();
            Projectile->FireInDirection(LaunchDirection);
        }

    
    
    //MultiCase Version
//    Multi_OnFire(Location, Rotation);
    
}

//void APlayerCharacter::OnFire()
//{
//    UE_LOG(LogTemp, Warning, TEXT("Function Called"));
//    // try and fire a projectile
//    if (ProjectileClass != nullptr)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("passed first if Called"));
//        UWorld* const World = GetWorld();
//        if (World != nullptr)
//        {
//                const FRotator SpawnRotation = GetControlRotation();
//                // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
////                const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
//                const FVector SpawnLocation(90.f, 40.0f, 50.0f);
//                FActorSpawnParameters ActorSpawnParams;
////                ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
//                ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//                World->SpawnActor<AMyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
//
//            UE_LOG(LogTemp, Warning, TEXT("Inside Big If"));
//
//        }
//    }
//
//    // try and play the sound if specified
//    if (FireSound != nullptr)
//    {
//        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
//    }
//
////    // try and play a firing animation if specified
////    if (FireAnimation != nullptr)
////    {
////        // Get the animation object for the arms mesh
////        UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
////        if (AnimInstance != nullptr)
////        {
////            AnimInstance->Montage_Play(FireAnimation, 1.f);
////        }
////    }
//}


// Player moves forwards or backwards
// Val is the movement amount input
void APlayerCharacter::MoveForward(float Val)
{
	FVector Direction = GetActorForwardVector();//FRotationMatrix(Controller->GetActorForwardVector()).GetScaledAxis(EAxis::X);
    // UE_LOG(LogTemp, Warning, TEXT("Forward is %d %d %d"), Direction.X, Direction.Y, Direction.Z);
//
	AddMovementInput(Direction, Val);
}

// Player moves left or right
void APlayerCharacter::Strafe(float Val)
{
	// FVector Direction = GetActorRightVector();	//FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    // AddMovementInput(Direction, Val);
	const FRotator Rotation = GetControlRotation();
	// const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get right vector
	const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
	    // UE_LOG(LogTemp, Warning, TEXT("Right is %d %d %d"), Direction.X, Direction.Y, Direction.Z);


	// add movement in that direction
	AddMovementInput(Direction, Val);
}

void APlayerCharacter::Ascend(float Val)
{
	FVector Direction = GetActorUpVector();
	AddMovementInput(Direction, Val);

}

// Player spins left or right
void APlayerCharacter::Spin(float Val)
{
	// if (Val)
		// UE_LOG(LogTemp, Warning, TEXT("val is %d"), Val);
	AddControllerRollInput(Val);
	if (Val)
		AddActorLocalRotation(FQuat(FRotator(0, 0, Val)));
}

void APlayerCharacter::HorizontalRotation(float Val)
{
	// UE_LOG(LogTemp, Warning, TEXT("left is %d"), Val);

		AddControllerYawInput(Val);
		// AddActorLocalRotation(FQuat(FRotator(0, Val, 0)));

		// add this later GetWorld()->GetDeltaSeconds()
}

void APlayerCharacter::VerticalRotation(float Val)
{
	// UE_LOG(LogTemp, Warning, TEXT("right is is %d"), Val);

	// if (Val)
	AddControllerPitchInput(Val);
		// AddActorLocalRotation(FQuat(FRotator(Val, 0, 0)));
}



