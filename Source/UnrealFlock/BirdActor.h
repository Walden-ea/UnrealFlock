// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

#include "BirdActor.generated.h"

UCLASS()
class UNREALFLOCK_API ABirdActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
		bool shouldMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector Velocity = FVector(0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		int CurrentTargetNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		TArray<FVector> Targets = TArray<FVector>({ FVector(0.0f),FVector(0.0f),FVector(0.0f) });

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MaxSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float SteeringMagnitude = 0.1f;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* VisualMesh;

	// Sets default values for this actor's properties
	ABirdActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	float PreviousRatio = 1;

	void PerformMovement();
	FVector CalcVelocity();
	float CalcArrival(float distance);
	FRotator CalcAngles();

};
