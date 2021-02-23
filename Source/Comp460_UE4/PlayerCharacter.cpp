// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Disables in built controller rotation
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using PlayerCharacter."));

	if (RootComponent != nullptr) {
		Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
		// Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		// Camera->AttachTo(RootComponent);
	}
	
	
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
}

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



