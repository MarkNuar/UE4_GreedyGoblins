// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat.h"

#include "DrawDebugHelpers.h"
#include "GreedyGoblins/GreedyGoblinsGameState.h"

// Sets default values
ABoat::ABoat()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	MovementComponent = CreateDefaultSubobject<UBoatMovementComponent>(TEXT("MovementComponent"));
	MovementReplicator = CreateDefaultSubobject<UBoatMovementReplicator>(TEXT("MovementReplicator"));
}

// Called when the game starts or when spawned
void ABoat::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(false);

	if(HasAuthority())
	{
		NetUpdateFrequency = 100;
	}
}

FString GetEnumText (ENetRole Role)
{
	switch(Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "Simulated Proxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "ERROR";
	}
}

// Called every frame
void ABoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(GetLocalRole()), this, FColor::White, DeltaTime);

	AGreedyGoblinsGameState* GreedyGoblinsGameState = Cast<AGreedyGoblinsGameState>(GetWorld()->GetGameState());
	if (!ensure(GreedyGoblinsGameState != nullptr)) return;
	
	if(GreedyGoblinsGameState->GetEnragedMode())
		DrawDebugString(GetWorld(), FVector(0, 0, 50), "Sail key presa!", this, FColor::White, DeltaTime);
	
}

// Called to bind functionality to input
void ABoat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABoat::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABoat::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this,  &ABoat::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ABoat::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ABoat::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &ABoat::LookRightRate);

	
}

void ABoat::MoveForward(float Value)
{
	if (MovementComponent == nullptr) return;
	MovementComponent->SetThrottle(Value);
}

void ABoat::MoveRight(float Value)
{
	if (MovementComponent == nullptr) return;
	MovementComponent->SetSteeringThrow(Value);
}

void ABoat::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ABoat::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void ABoat::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds()); 
}

void ABoat::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds()); 
}
