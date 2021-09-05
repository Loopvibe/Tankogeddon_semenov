#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Cannon.generated.h"


class UArrowComponent;
class AProjectile;

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* ProjectileSpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* ShootEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* AudioEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRate = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRange = 1000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireDamage = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		int32 MaxAmmo = 25;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		ECannonType Type = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		TSubclassOf<AProjectile> ProjectileClass;

	FTimerHandle ReloadTimerHandle;

	bool ReadyToFire = false;
	int32 NumAmmo = 0;

public:
	ACannon();

	void AddAmmo(int32 ammo);
	
	void Fire();

	void FireSpecial();

	bool IsReadyToFire();

	void SetVisibility(bool bIsVisibility);

protected:
	virtual void BeginPlay() override;

	void Reload();
};


