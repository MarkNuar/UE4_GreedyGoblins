// Fill out your copyright notice in the Description page of Project Settings.


#include "LightHouse.h"

// Sets default values
ALightHouse::ALightHouse()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	if(!ensure(Root)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *Root->GetName());
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	if(!ensure(StaticMeshComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *StaticMeshComponent->GetName());
	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLightComponent->SetupAttachment(StaticMeshComponent);
	if(!ensure(SpotLightComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *SpotLightComponent->GetName());
	PatrolTargetComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PatrolTargetComponent"));
	PatrolTargetComponent->SetupAttachment(RootComponent);
	if(!ensure(PatrolTargetComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *PatrolTargetComponent->GetName());
}

// Called when the game starts or when spawned
void ALightHouse::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightHouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector Direction = - SpotLightComponent->GetComponentLocation() + PatrolTargetComponent->GetComponentLocation();
	const FRotator Rotation = Direction.Rotation();
	SpotLightComponent->SetWorldRotation(Rotation);
}

// Called to bind functionality to input
void ALightHouse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

