// Fill out your copyright notice in the Description page of Project Settings.



#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

class IDamageTaker;

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	DamageEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Damage Effect"));
	DamageEffect->SetupAttachment(RootComponent);
	DestroyEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Destroy Effect"));
	DestroyEffect->SetupAttachment(RootComponent);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect"));
	AudioEffect->SetupAttachment(RootComponent);
	


}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
	SetLifeSpan(FlyRange / MoveSpeed);
}


void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor,
		UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AActor* owner = GetOwner();
	AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;
	if (OtherActor != owner && OtherActor != ownerByOwner)
	{
		IDamageTaker* damageTakerActor = Cast<IDamageTaker>(OtherActor);
		if (damageTakerActor)
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;
			damageTakerActor->TakeDamage(damageData);
		}
		else
		{
			UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
			if (mesh)
			{
				if (mesh->IsSimulatingPhysics())
				{
					FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
					forceVector.Normalize();
					mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
				}
			}
		}
        Explode();
		Destroy();
	}
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}

void AProjectile::Explode()
{
    if (!VolumetricExplosion)
    {
        return;
    }

    FVector startPos = GetActorLocation();
    FVector endPos = startPos + FVector(0.1f);

    FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
    FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
    params.AddIgnoredActor(this);
    params.bTraceComplex = true;
    params.TraceTag = "Explode Trace";
    TArray<FHitResult> AttackHit;

    FQuat Rotation = FQuat::Identity;

    bool sweepResult = GetWorld()->SweepMultiByChannel
    (
        AttackHit,
        startPos,
        endPos,
        Rotation,
        ECollisionChannel::ECC_Visibility,
        Shape,
        params
    );

    GetWorld()->DebugDrawTraceTag = "Explode Trace";

    if (sweepResult)
    {
        for (FHitResult hitResult : AttackHit)
        {
            AActor* otherActor = hitResult.GetActor();
            if (!otherActor)
                continue;

            IDamageTaker* damageTakerActor = Cast<IDamageTaker>(otherActor);
            if (damageTakerActor)
            {
                FDamageData damageData;
                damageData.DamageValue = Damage;
                damageData.Instigator = GetOwner();
                damageData.DamageMaker = this;

                damageTakerActor->TakeDamage(damageData);
            }
            else
            {
                UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(otherActor->GetRootComponent());
                if (mesh)
                {
                    if (mesh->IsSimulatingPhysics())
                    {
                        FVector forceVector = otherActor->GetActorLocation() - GetActorLocation();
                        forceVector.Normalize();
                        mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
                    }
                }
            }

        }
    }
}