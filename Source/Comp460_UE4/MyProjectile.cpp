// Fill out your copyright notice in the Description page of Project Settings.
#include "MyProjectile.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"



// Sets default values
AMyProjectile::AMyProjectile()
{
    // Die after 3 seconds by default
    InitialLifeSpan = 1.0f;

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    //Use a sphere as a simple collision representation
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

    // Set the sphere's collision profile name to "Projectile".
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

    // Event called when component hits something.
    CollisionComponent->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);

    //Set the sphere's collisiion radius.
    CollisionComponent->InitSphereRadius(15.0f);

    //Set the root component to be the collision component
    RootComponent = CollisionComponent;

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 4000.f;
    ProjectileMovementComponent->MaxSpeed = 4000.f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.5f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

    ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Blueprints/Sphere.Sphere'"));
    if (Mesh.Succeeded())
    {
        ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Materials/Items/SphereMaterial.SphereMaterial'"));
    if (Material.Succeeded())
    {
        ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
    }
    ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
    ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
    ProjectileMeshComponent->SetupAttachment(RootComponent);

    bReplicates = true;
}

// Called when the game starts or when spawned
void AMyProjectile::BeginPlay()
{
    Super::BeginPlay();

}

// Called when the game starts or when spawned
void AMyProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    // Only add impulse and destroy projectile if we hit a physics
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComponent != nullptr) && OtherComponent->IsSimulatingPhysics())
    {
        OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
        UE_LOG(LogTemp, Warning, TEXT("HIT A PHYSICS OBJECT"));
        Destroy();
    }
    else if (APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("HIT A Player"));
        Destroy();
    }
}

void AMyProjectile::HitPlayer(AActor* HitActor) {
    UE_LOG(LogTemp, Warning, TEXT("YOu hit a player"));
}

// Function that initializes the projectile's velocity in the shoot direction.
void AMyProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

