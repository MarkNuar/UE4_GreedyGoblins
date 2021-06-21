// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SailKey.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PearlOfDestiny.generated.h"

UCLASS()
class GREEDYGOBLINS_API APearlOfDestiny : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APearlOfDestiny();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ShieldMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* PearlTrigger;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PearlMesh;

public:
	UStaticMeshComponent* GetShieldMesh() const
	{
		return ShieldMesh;
	}

	UPROPERTY()
	AGreedyGoblinsGameState* GreedyGoblinsGameState;
	
private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TArray<AActor*> SailKeySpawnPoints;

	TSubclassOf<class ASailKey> SailKeyClass;
};
