// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Cannon.h"
#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;


UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Tank
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;
	//camera
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;
	//Helth
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;
	//seting
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 400;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 170;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float InterpolationKey = 0.05f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Speed")
		float TurretRotationInterpolationKey = 0.05f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* CannonSetupPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> CannonClass;
	UPROPERTY()
		ACannon* ActiveCannon;
	UPROPERTY()
		ACannon* InactiveCannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
		TArray<ATargetPoint*> PatrollingPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
		float MovementAccurency = 50;


	float TargetForwardAxisValue;
	float TargetRightAxisValue;
	float CurrentRightAxisValue;

	UPROPERTY()
		ATankPlayerController* TankController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UFUNCTION()
		void MoveForward(float AxisValue);

	UFUNCTION()
		void RotateRight(float AxisValue);

	UFUNCTION()
		void Fire();

	UFUNCTION()
		void FireSpecial();

	UFUNCTION()
	void SetupCannon(TSubclassOf<ACannon> InCannonClass);

	UFUNCTION()
		void SwapCannon();

	UFUNCTION()
		ACannon* GetActiveCannon() const;

	UFUNCTION()
		void TakeDamage(FDamageData DamageData);

	UFUNCTION()
		float GetMovementAccurency() { return MovementAccurency; };

	UFUNCTION()
		FVector GetTurretForwardVector();

	UFUNCTION()
		void RotateTurretTo(FVector TargetPosition);

	FVector GetEyesPosition();

	UFUNCTION()
	TArray<FVector> GetPatrollingPoints();

	UFUNCTION()
	void SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints);


	UFUNCTION(BlueprintImplementableEvent)
		void PlayerDie();


protected:
	
	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaked(float DamageValue);


};
