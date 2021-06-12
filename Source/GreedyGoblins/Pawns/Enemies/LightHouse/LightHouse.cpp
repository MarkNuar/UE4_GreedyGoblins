// Fill out your copyright notice in the Description page of Project Settings.


#include "LightHouse.h"

#include "Components/SphereComponent.h"
#include "GreedyGoblins/Pawns/Boat/Boat.h"

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

	//TODO If Has authority
	PatrolTargetComponent->OnComponentBeginOverlap.AddDynamic(this, &ALightHouse::OnOverlapBegin);
	PatrolTargetComponent->OnComponentEndOverlap.AddDynamic(this, &ALightHouse::OnOverlapEnd);

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
	if(!ensure(SpotLightComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *SpotLightComponent->GetName());
	const FVector Direction = PatrolTargetComponent->GetComponentLocation() - SpotLightComponent->GetComponentLocation();
	const FRotator Rotation = Direction.Rotation();
	SpotLightComponent->SetWorldRotation(Rotation);
	
}

// Called to bind functionality to input
void ALightHouse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALightHouse::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!ensure(OtherActor)) return;
	if(!ensure(OtherComp)) return;
	
	if(Cast<USphereComponent>(OtherComp) && BoatToChase == nullptr)
	{
		if(!ensure(Cast<ABoat>(OtherActor))) return;
		BoatToChase = Cast<ABoat>(OtherActor);
	}
		
}

void ALightHouse::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!ensure(OtherActor)) return;
	if(!ensure(OtherComp)) return;

	if(Cast<USphereComponent>(OtherComp))
	{
		ABoat* OtherBoat = Cast<ABoat>(OtherActor);
		if(!ensure(OtherBoat)) return;
		
		if(BoatToChase == OtherBoat)
		{
			BoatToChase = nullptr;
		}
	}

}



