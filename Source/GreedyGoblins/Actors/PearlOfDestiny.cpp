// Fill out your copyright notice in the Description page of Project Settings.


#include "PearlOfDestiny.h"

#include "GreedyGoblins/GreedyGoblinsGameInstance.h"

// Sets default values
APearlOfDestiny::APearlOfDestiny()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PearlTrigger= CreateDefaultSubobject<UBoxComponent>(FName("PearlTrigger"));
	if (!ensure(PearlTrigger != nullptr)) return;
	PearlMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PearlMesh"));
	if (!ensure(PearlMesh != nullptr)) return;
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ShieldMesh"));
	if (!ensure(ShieldMesh != nullptr)) return;

	PearlMesh->SetupAttachment(PearlTrigger);
	ShieldMesh->SetupAttachment(PearlMesh);

	PearlTrigger->OnComponentBeginOverlap.AddDynamic(this, &APearlOfDestiny::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APearlOfDestiny::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APearlOfDestiny::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APearlOfDestiny::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Volevi"));
	if(OtherComp->GetClass()->IsChildOf(UBoxComponent::StaticClass()) && OverlappedComp->GetClass()->IsChildOf(UBoxComponent::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Mi hai preso"));
		UGreedyGoblinsGameInstance* GameInstance = Cast<UGreedyGoblinsGameInstance>(GetGameInstance());
		if(!ensure(GameInstance)) return;
		GameInstance->LoadEndGameMenu();
	}
}

