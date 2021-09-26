// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "DamageTaker.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Projectile.generated.h"



UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UStaticMeshComponent* Mesh;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UParticleSystemComponent* DamageEffect;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UParticleSystemComponent* DestroyEffect;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        UAudioComponent* AudioEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
        float FlyRange = 5000;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
        float MoveSpeed = 100;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
        float MoveRate = 0.005f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        bool VolumetricExplosion=true;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        float Damage = 1;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        float ExplodeRadius = 5.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        float PushForce = 1000;

    FTimerHandle MovementTimerHandle;

public:
    AProjectile();

    virtual void Start();

protected:
    UFUNCTION()
        void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
      virtual  void Move();

    UFUNCTION()
      void Explode();
};
