// Fill out your copyright notice in the Description page of Project Settings.


#include "LightHouse.h"

// Sets default values
ALightHouse::ALightHouse()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if(!ensure(Root)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *Root->GetName());
	RootComponent = Root;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if(!ensure(StaticMeshComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *StaticMeshComponent->GetName());
	StaticMeshComponent->SetupAttachment(RootComponent);
	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	if(!ensure(SpotLightComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *SpotLightComponent->GetName());
	SpotLightComponent->SetupAttachment(StaticMeshComponent);
	PatrolTargetComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PatrolTargetComponent"));
	if(!ensure(PatrolTargetComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *PatrolTargetComponent->GetName());
	PatrolTargetComponent->SetupAttachment(RootComponent);
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	if(!ensure(SplineComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *SplineComponent->GetName());
	SplineComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALightHouse::BeginPlay()
{
	Super::BeginPlay();
	if(!ensure(PatrolTargetComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *PatrolTargetComponent->GetName());
	if(!ensure(SplineComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *SplineComponent->GetName());
	PatrolTargetComponent->SetWorldLocation(SplineComponent->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World));
}

float ALightHouse::TimeInterp(float DeltaTime){
	
	if(bIsAlphaIncreasing){
		AlphaMovement += LightSpeed*DeltaTime;
		if(AlphaMovement>=1-KINDA_SMALL_NUMBER){
			bIsAlphaIncreasing = false;
		}
	}else{
		AlphaMovement -= LightSpeed*DeltaTime;
		if(AlphaMovement<=KINDA_SMALL_NUMBER){
			bIsAlphaIncreasing = true;
		}
	}
	return AlphaMovement;
}

// Called every frame
void ALightHouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!ensure(SpotLightComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *SpotLightComponent->GetName());
	const FVector Direction = - SpotLightComponent->GetComponentLocation() + PatrolTargetComponent->GetComponentLocation();
	const FRotator Rotation = Direction.Rotation();
	SpotLightComponent->SetWorldRotation(Rotation);
	
	if(!ensure(SplineComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *SplineComponent->GetName());
	if(!ensure(PatrolTargetComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *PatrolTargetComponent->GetName());
	
	float InterpPosition = TimeInterp(DeltaTime);
	PatrolTargetComponent->SetWorldLocation(SplineComponent->GetLocationAtDistanceAlongSpline(
		InterpPosition*SplineComponent->GetSplineLength(), ESplineCoordinateSpace::World
		));
}

// Called to bind functionality to input
void ALightHouse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

