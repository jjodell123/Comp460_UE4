// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class COMP460_UE4_API AMyProjectile : public AActor
{
    GENERATED_BODY()


public:
    // Sets default values for this actor's properties
    AMyProjectile();

protected:
    // Called when game starts or when spawned
    virtual void BeginPlay() override;

public:

    /** Sphere collision component*/
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
        USphereComponent* CollisionComponent;

    /** Projectile movement component*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
        UProjectileMovementComponent* ProjectileMovementComponent;

    // Projectile mesh
    UPROPERTY(EditAnywhere, Category = Projectile)
        UStaticMeshComponent* ProjectileMeshComponent;

    // Projectile Material
    UPROPERTY(EditAnywhere, Category = Movement)
        UMaterialInstanceDynamic* ProjectileMaterialInstance;

    // Damage amount
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Variables")
        int damage;

    UFUNCTION(BlueprintCallable)
        void HitPlayer(AActor* HitActor);

    /** called when projectile hits something */
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    

    // Function that initializes the projectile's velocity in the shoot direction.
    void FireInDirection(const FVector& ShootDirection);

    //    /** Returns CollisionComp subobject **/
    //    USphereComponent* GetCollisionComp() const { return CollisionComp; }
    //    /** Returns ProjectileMovement subobject **/
    //    UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

};