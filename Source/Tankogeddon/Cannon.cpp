#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"


ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot Effect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect"));
	AudioEffect->SetupAttachment(ProjectileSpawnPoint);
}



void ACannon::Fire()
{
	if (!ReadyToFire || NumAmmo <= 0)
	{
		return;
	}
	ReadyToFire = false;

	ShootEffect->ActivateSystem();
	AudioEffect->Play();

	--NumAmmo;

	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - projectile");

		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (projectile)
		{
			projectile->Start();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");
		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;

		FVector start = ProjectileSpawnPoint->GetComponentLocation();
		FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			if (hitResult.Actor.Get())
			{
				hitResult.Actor.Get()->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f, 0, 5);
		}

	}
	
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}



void ACannon::FireSpecial()
{
	if (!ReadyToFire || NumAmmo<=0)
	{
		return;
	}
	ReadyToFire = false;

	--NumAmmo;

	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "FireSpecial - projectile");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "FireSpecial - trace");
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

bool ACannon::IsReadyToFire()
{
	return ReadyToFire;
}

void ACannon::SetVisibility(bool bIsVisibility)
{

	Mesh->SetHiddenInGame(!bIsVisibility);

}

void ACannon::AddAmmo(int32 ammo)
{
	if ((ammo+ NumAmmo)>= MaxAmmo)
	{
		NumAmmo = MaxAmmo;
	}
	else
	{
		NumAmmo = NumAmmo + ammo;
	}
}

void ACannon::Reload()
{
	ReadyToFire = true;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
	NumAmmo = MaxAmmo;
}



