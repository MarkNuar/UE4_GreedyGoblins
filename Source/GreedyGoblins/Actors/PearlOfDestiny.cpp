// Fill out your copyright notice in the Description page of Project Settings.


#include "PearlOfDestiny.h"

// Sets default values
APearlOfDestiny::APearlOfDestiny()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PearlTrigger= CreateDefaultSubobject<UBoxComponent>(FName("PearlTrigger"));
	if (!ensure(PearlTrigger != nullptr)) return;
	PearlMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PearlMesh"));
	if (!ensure(PearlMesh != nullptr)) return;
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ShieldMesh"));
	if (!ensure(ShieldMesh != nullptr)) return;

	PearlMesh->SetupAttachment(PearlTrigger);
	ShieldMesh->SetupAttachment(PearlMesh);

	if(HasAuthority())
	{
		PearlTrigger->OnComponentBeginOverlap.AddDynamic(this, &APearlOfDestiny::OnOverlapBegin);
	}
	
	const ConstructorHelpers::FClassFinder<ASailKey> SailKeyBPClass(TEXT("/Game/Blueprint/Actors/BP_SailKey"));
	if(!ensure(SailKeyBPClass.Class!=nullptr)) return;
	SailKeyClass = SailKeyBPClass.Class;
	
}

// Called when the game starts or when spawned
void APearlOfDestiny::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		if(!ensure(SailKeySpawnPoints.Num() != 0)) return;

		const FVector Location = SailKeySpawnPoints[FMath::RandRange(0, SailKeySpawnPoints.Num()-1)]->GetActorLocation();
		GetWorld()->SpawnActor<ASailKey>(SailKeyClass, Location, FRotator(0, 0, 0));
		
	}

	GreedyGoblinsGameState = Cast<AGreedyGoblinsGameState>(GetWorld()->GetGameState());
	if (!ensure(GreedyGoblinsGameState != nullptr)) return;
}


void APearlOfDestiny::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherComp->GetClass()->IsChildOf(UBoxComponent::StaticClass()) && OverlappedComp->GetClass()->IsChildOf(UBoxComponent::StaticClass()))
	{
		GreedyGoblinsGameState->SetIsGameEnded(true);
		GreedyGoblinsGameState->ShowEndScreen();
	}
}



