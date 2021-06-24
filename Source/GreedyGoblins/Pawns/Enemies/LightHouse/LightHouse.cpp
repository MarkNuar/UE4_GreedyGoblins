// Fill out your copyright notice in the Description page of Project Settings.


#include "LightHouse.h"

#include "Components/SphereComponent.h"
#include "GreedyGoblins/Pawns/Boat/Boat.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ALightHouse::ALightHouse()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if(!ensure(StaticMeshComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *StaticMeshComponent->GetName());
	RootComponent = StaticMeshComponent;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	if(!ensure(SplineComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *SplineComponent->GetName());
	SplineComponent->SetupAttachment(RootComponent);
	
	PatrolTargetTransform = CreateDefaultSubobject<USceneComponent>(TEXT("PatrolTargetTransform"));
	if(!ensure(PatrolTargetTransform)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *PatrolTargetTransform->GetName());
	PatrolTargetTransform->SetupAttachment(RootComponent);
	
	EyeSocket = CreateDefaultSubobject<USceneComponent>(TEXT("EyeSocket"));
	if(!ensure(EyeSocket)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *EyeSocket->GetName());
	EyeSocket->SetupAttachment(RootComponent);
	
	LightConeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightConeMeshComponent"));
	if(!ensure(LightConeMeshComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *LightConeMeshComponent->GetName());
	LightConeMeshComponent->SetupAttachment(EyeSocket);

	MovementReplicator = CreateDefaultSubobject<ULighthouseMovementReplicator>(TEXT("LightHouseMovementReplicator"));
	if(!ensure(MovementReplicator != nullptr)) return;
	MovementReplicator->SetIsReplicated(true);

}

// Called when the game starts or when spawned
void ALightHouse::BeginPlay()
{
	Super::BeginPlay();
	
	LightConeDynamicMaterial = UMaterialInstanceDynamic::Create(LightConeMeshComponent->GetMaterial(0), LightConeMeshComponent);
	
	if(HasAuthority())
	{
		LightConeMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ALightHouse::OnOverlapBegin);
		LightConeMeshComponent->OnComponentEndOverlap.AddDynamic(this, &ALightHouse::OnOverlapEnd);	
	}

	UpdateLightColor();
}

// Called every frame
void ALightHouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!ensure(EyeSocket)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *EyeSocket->GetName());
	const FVector Direction = PatrolTargetTransform->GetComponentLocation() - EyeSocket->GetComponentLocation();
	const FRotator Rotation = Direction.Rotation();
	EyeSocket->SetWorldRotation(Rotation);
}

// Called to bind functionality to input
void ALightHouse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector ALightHouse::GetPositionAlongSpline() const
{
	return SplineComponent->FindLocationClosestToWorldLocation(PatrolTargetTransform->GetComponentLocation(), ESplineCoordinateSpace::World);	
}

void ALightHouse::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!ensure(OtherActor)) return;
	if(!ensure(OtherComp)) return;

	if(OtherComp->GetClass()->IsChildOf(USphereComponent::StaticClass()) && BoatToChase == nullptr)
	{
		if(!ensure(Cast<ABoat>(OtherActor))) return;
		BoatToChase = Cast<ABoat>(OtherActor);
	}

	if(OtherComp->GetClass()->IsChildOf(UBoxComponent::StaticClass()) && OtherActor->GetClass()->IsChildOf(ABoat::StaticClass()))
	{
		ABoat* BoatInLight = Cast<ABoat>(OtherActor);
		if(!ensure(BoatInLight)) return;
		BoatInLight->Caught();
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

void ALightHouse::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALightHouse, bIsChasing);
}

void ALightHouse::On_bIsChasing_Change()
{
	UpdateLightColor();
}

void ALightHouse::SetIsInEnragedMode(bool bLocalIsInEnragedMode)
{
	bIsInEnragedMode = bLocalIsInEnragedMode;
	if(bIsInEnragedMode)
	{
		LightSpeed = EnragedLightSpeed;
	}
	else
	{
		LightSpeed = BaseLightSpeed;
	}
	UpdateLightColor();
}

void ALightHouse::UpdateLightColor()
{
	if(bIsChasing)
	{
		FLinearColor Color = FLinearColor::Red;
		LightConeDynamicMaterial->SetVectorParameterValue("LightColor", Color);
		LightConeMeshComponent->SetMaterial(0, LightConeDynamicMaterial);
	}
	else
	{
		if(bIsInEnragedMode)
		{
			FLinearColor Color = FLinearColor::Yellow;
			LightConeDynamicMaterial->SetVectorParameterValue("LightColor", Color);
			LightConeMeshComponent->SetMaterial(0, LightConeDynamicMaterial);
		}
		else
		{
			FLinearColor Color = FLinearColor::Green;
			LightConeDynamicMaterial->SetVectorParameterValue("LightColor", Color);
			LightConeMeshComponent->SetMaterial(0, LightConeDynamicMaterial);
		}
	}
}




