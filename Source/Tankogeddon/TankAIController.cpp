// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


void ATankAIController::Initialize()
{
    TankPawn = Cast<ATankPawn>(GetPawn());
    if (TankPawn)
    {
        PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
        FVector pawnLocation = TankPawn->GetActorLocation();
        MovementAccurency = TankPawn->GetMovementAccurency();
        TArray<FVector> points = TankPawn->GetPatrollingPoints();
        
        for (FVector point : points)
        {
            PatrollingPoints.Add(point);
        }
        
        CurrentPatrolPointIndex = PatrollingPoints.Num() == 0 ? INDEX_NONE : 0;
    }
}

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();
    Initialize();

}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!TankPawn)
        Initialize();

    if (!TankPawn)
        return;

    if (CurrentPatrolPointIndex == INDEX_NONE)
    {
        TankPawn->MoveForward(0.f);
        return;
    }

    TankPawn->MoveForward(1.f);
    FVector CurrentPoint = PatrollingPoints[CurrentPatrolPointIndex];
    FVector PawnLocation = TankPawn->GetActorLocation();
    if (FVector::Distance(CurrentPoint, PawnLocation) <= MovementAccurency)
    {
        CurrentPatrolPointIndex++;
        if (CurrentPatrolPointIndex >= PatrollingPoints.Num())
        {
            CurrentPatrolPointIndex = 0;
        }
    }

    FVector MoveDirection = CurrentPoint - PawnLocation;
    MoveDirection.Normalize();
    FVector ForwardDirection = TankPawn->GetActorForwardVector();
    FVector RightDirection = TankPawn->GetActorRightVector();

    DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

    float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, MoveDirection)));
    float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, MoveDirection)));

    float RotationValue = 0;
    if (ForwardAngle > 5.f)
    {
        RotationValue = 1.f;
    }
    if (RightAngle > 90.f)
    {
        RotationValue = -RotationValue;
    }

    TankPawn->RotateRight(RotationValue);

    Targeting();
}

float ATankAIController::GetRotationValue()
{
    FVector currentPoint = PatrollingPoints[CurrentPatrolPointIndex];
    FVector pawnLocation = TankPawn->GetActorLocation();
    if (FVector::Distance(currentPoint, pawnLocation) <= MovementAccurency)
    {
        CurrentPatrolPointIndex++;
        if (CurrentPatrolPointIndex >= PatrollingPoints.Num())
            CurrentPatrolPointIndex = 0;
    }

    FVector moveDirection = currentPoint - pawnLocation;
    moveDirection.Normalize();
    FVector forwardDirection = TankPawn->GetActorForwardVector();
    FVector rightDirection = TankPawn->GetActorRightVector();

    //DrawDebugLine(GetWorld(), pawnLocation, currentPoint, FColor::Green, false, 0.1f, 0, 5);

    float forwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(forwardDirection, moveDirection)));
    float rightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(rightDirection, moveDirection)));

    float rotationValue = 0;
    if (forwardAngle > 5)
        rotationValue = 1;
    if (rightAngle > 90)
        rotationValue = -rotationValue;

    return rotationValue;
}

void ATankAIController::Targeting()
{
    if (CanFire())
    {
        Fire();
    }
    else
    {
        RotateToPlayer();
    }
}

void ATankAIController::RotateToPlayer()
{
    if (IsPlayerInRange())
    {
        TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
    }
}

bool ATankAIController::IsPlayerInRange()
{
    return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;
}

bool ATankAIController::CanFire()
{
    
    if (!IsPlayerSeen())
        return false;
     

    FVector targetingDir = TankPawn->GetTurretForwardVector();
    FVector dirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
    dirToPlayer.Normalize();
    float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));
    return aimAngle <= Accurency;
}

bool ATankAIController::IsPlayerSeen()
{
    if (!PlayerPawn)
        Initialize();

    FVector playerPos = PlayerPawn->GetActorLocation();
    FVector eyesPos = TankPawn->GetEyesPosition();

    FHitResult hitResult;
    FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
    traceParams.bTraceComplex = true;
    traceParams.AddIgnoredActor(TankPawn);
    traceParams.bReturnPhysicalMaterial = false;

    if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, traceParams))
    {

        if (hitResult.Actor.Get() && hitResult.Actor.Get() != PlayerPawn)
        {
            DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
            return false;
        }
    }
    DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Cyan, false, 0.5f, 0, 10);
    DrawDebugCircle(GetWorld(), eyesPos, TargetingRange, 50, FColor::Red, false, 0.5f, 0, 10, FVector((1), (0), (0)), FVector((0), (1), (0)), true);

    return true;
}

void ATankAIController::Fire()
{
    TankPawn->Fire();
}


