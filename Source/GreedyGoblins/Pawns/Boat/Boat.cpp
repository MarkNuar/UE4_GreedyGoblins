// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat.h"

#include <assert.h>

#include "DrawDebugHelpers.h"
#include "ToolBuilderUtil.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "GreedyGoblins/GreedyGoblinsGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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

	if(ShowLightCylinder)
	{
		PlayerWithSailKeyLightCylinderMesh->SetHiddenInGame(false);
		DrawDebugString(GetWorld(), FVector(0, 0, -20), "I HAVE THE SAIL KEY AGAGGAHAGAGGGA", this, FColor::White, DeltaTime);
		
		UEngine* Engine = GetGameInstance()->GetEngine();
		if(!ensure(Engine!=nullptr)) return;
		Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("PLAYER NUM " + FString::FromInt(GetPlayerState()->GetPlayerId()) + "has the sail key"));
		Engine->AddOnScreenDebugMessage(1, 3, FColor::Green, TEXT("PLAYER NUM " + FString::FromInt(GetPlayerState()->GetPlayerId()) + "has the sail key"));
		Engine->AddOnScreenDebugMessage(2, 3, FColor::Green, TEXT("PLAYER NUM " + FString::FromInt(GetPlayerState()->GetPlayerId()) + "has the sail key"));
	}
	else
	{
		PlayerWithSailKeyLightCylinderMesh->SetHiddenInGame(true);
	}
	
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

	FVector CurrentActorPosition = GetActorLocation();

	MovementComponent->SetVelocity(FVector::ZeroVector);

	// TArray<AActor*> FoundActors;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	// // todo SHUFFLE PLAYER START LIST
	// APlayerStart* Spawn = nullptr;
	//
	// for(AActor* Actor : FoundActors)
	// {
	// 	Spawn = Cast<APlayerStart>(Actor);
	// 	if(!ensure(Spawn)) return;
	// 	UCapsuleComponent* Capsule = Spawn->GetCapsuleComponent();
	// 	if(!ensure(Capsule)) return;
	// 	const TArray<FOverlapInfo> OverlapInfos = Capsule->GetOverlapInfos();
	// 	UE_LOG(LogTemp, Warning, TEXT("%d"), OverlapInfos.Num());
	// 	if(OverlapInfos.Num() == 0)
	// 		break;
	// }

	AActor* FreeSpawn = GetWorld()->GetAuthGameMode()->ChoosePlayerStart(GetController());

	if(!ensure(FreeSpawn)) return;
	this->SetActorTransform(FreeSpawn->GetTransform());

	if(GreedyGoblinsGameState->HasSailKey(GetPlayerState()))
	{
		GreedyGoblinsGameState->DropSailKeyAtLocation(CurrentActorPosition);
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

