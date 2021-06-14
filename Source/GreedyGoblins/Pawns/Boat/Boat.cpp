// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat.h"

#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
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

	BoxCollider = FindComponentByClass<UBoxComponent>();
	if(!ensure(BoxCollider != nullptr)) return;
			
	SetReplicateMovement(false);

	if(HasAuthority()) // Executed only by the server (It's executed ON clients too, but not BY them: still by the server)
	{
		NetUpdateFrequency = 100;
		BoxCollider->OnComponentHit.AddDynamic(this, &ABoat::OnBoatHit);
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

	if(GetPlayerState() != nullptr)
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 20), "Player number " + FString::FromInt(GetPlayerState()->GetPlayerId()),this, FColor::White, DeltaTime);
	}
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

void ABoat::OnBoatHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->GetClass()->IsChildOf(this->StaticClass()))
	{
		AGreedyGoblinsGameState* GreedyGoblinsGameState = Cast<AGreedyGoblinsGameState>(GetWorld()->GetGameState());
		if (!ensure(GreedyGoblinsGameState != nullptr)) return;
		
		APawn* PlayerHitPawn = Cast<APawn>(OtherActor);
		if (!ensure(PlayerHitPawn != nullptr)) return;
		if(GreedyGoblinsGameState->HasSailKey(PlayerHitPawn->GetPlayerState()) && GreedyGoblinsGameState->GetCanGetSailKey())
		{	
			GreedyGoblinsGameState->UpdateSailKeyOwner(GetPlayerState());
		}
	}

}

