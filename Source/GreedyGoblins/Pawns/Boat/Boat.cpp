// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat.h"

#include <assert.h>

#include "BoatController.h"
#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GreedyGoblins/GreedyGoblinsGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

class APlayerStartPIE;
// Sets default values
ABoat::ABoat()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	MovementComponent = CreateDefaultSubobject<UBoatMovementComponent>(TEXT("MovementComponent"));
	if(!ensure(MovementComponent != nullptr)) return;
	MovementReplicator = CreateDefaultSubobject<UBoatMovementReplicator>(TEXT("MovementReplicator"));
	if(!ensure(MovementReplicator != nullptr)) return;
	MovementReplicator->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ABoat::BeginPlay()
{
	Super::BeginPlay();

	BoxCollider = FindComponentByClass<UBoxComponent>();
	if(!ensure(BoxCollider != nullptr)) return;
	PlayerWithSailKeyLightCylinderMesh = FindComponentByClass<UStaticMeshComponent>();
	if(!ensure(PlayerWithSailKeyLightCylinderMesh != nullptr)) return;
	GreedyGoblinsGameState = Cast<AGreedyGoblinsGameState>(GetWorld()->GetGameState());
	if (!ensure(GreedyGoblinsGameState != nullptr)) return;
	
	SetReplicateMovement(false);

	if(HasAuthority()) // Executed only by the server (It's executed ON clients too, but not BY them: still by the server)
	{
		BoxCollider->OnComponentHit.AddDynamic(this, &ABoat::OnBoatHit);
		if(IsLocallyControlled())
		{
			SetAutonomousProxy(false);
		}
	}

	if(IsLocallyControlled())
	{
		BoatController = Cast<ABoatController>(GetController());
		if(!ensure(BoatController)) return;
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

	// DrawDebugString(GetWorld(), FVector(0, 0, 500), "Local Role: " + GetEnumText(GetLocalRole()),this, FColor::Green, DeltaTime);
	// DrawDebugString(GetWorld(), FVector(0, 0, 600), "Remote Role: " + GetEnumText(GetRemoteRole()),this, FColor::Green, DeltaTime);

	if(ShowLightCylinder)
	{
		PlayerWithSailKeyLightCylinderMesh->SetHiddenInGame(false);
	}
	else
	{
		PlayerWithSailKeyLightCylinderMesh->SetHiddenInGame(true);
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
	PlayerInputComponent->BindAction(TEXT("FastMode"), IE_Pressed,this, &ABoat::ToggleFastMode);
}

void ABoat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABoat, ShowLightCylinder);
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

void ABoat::ToggleFastMode()
{
	if (MovementComponent == nullptr) return;
	MovementComponent->ToggleFastMode();
	if(BoatController)
	{
		BoatController->ToggleFastMode(MovementComponent->IsInFastMode()); // For UI changes
	}
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

void ABoat::Caught()
{
	assert(HasAuthority()); // this should be true

	const FVector CurrentActorPosition = GetActorLocation();

	MovementComponent->SetVelocity(FVector::ZeroVector);
	AActor* FreeSpawn = GetWorld()->GetAuthGameMode()->FindPlayerStart(GetController());
	// AActor* FreeSpawn = GetRandomPlayerStart();
	if(!ensure(FreeSpawn)) return;
	this->SetActorTransform(FreeSpawn->GetTransform());

	if(GreedyGoblinsGameState->HasSailKey(GetPlayerState()))
	{
		GreedyGoblinsGameState->DropSailKeyAtLocation(CurrentActorPosition);
	}
}

AActor* ABoat::GetRandomPlayerStart()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	while(true)
	{
		AActor* PlayerStart = PlayerStarts[FMath::RandRange(0, PlayerStarts.Num()-1)];
		PlayerStart->SetActorEnableCollision(true);
		TArray<AActor*> OverlappingActors;
		PlayerStart->GetOverlappingActors(OverlappingActors);
		if(OverlappingActors.Num() == 0)
		{
			return PlayerStart;
		}
	}
}

void ABoat::OnBoatHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->GetClass()->IsChildOf(this->StaticClass()))
	{
		APawn* PlayerHitPawn = Cast<APawn>(OtherActor);
		if (!ensure(PlayerHitPawn != nullptr)) return;
		if(GreedyGoblinsGameState->HasSailKey(PlayerHitPawn->GetPlayerState()) && GreedyGoblinsGameState->GetCanGetSailKey())
		{	
			GreedyGoblinsGameState->UpdateSailKeyOwner(PlayerHitPawn->GetPlayerState(),GetPlayerState());
		}
	}
}

