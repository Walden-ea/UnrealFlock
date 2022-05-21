// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdActor.h"

// Sets default values
ABirdActor::ABirdActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeVisualAsset(TEXT("/Game/Assets/Shapes/Shape_Cone.Shape_Cone"));

	if (ConeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(ConeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	}
}

// Called when the game starts or when spawned
void ABirdActor::BeginPlay()
{
	Super::BeginPlay();
	Velocity = FVector(FMath::RandRange(-10, 10), FMath::RandRange(10, 10), FMath::RandRange(-10, 10));

	MaxSpeed = Velocity.Size();

	CurrentTargetNumber = 0;

	PreviousRatio = 1;
	
}

// Called every frame
void ABirdActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Velocity = CalcVelocity();

	if (shouldMove)
	{
		PerformMovement();
	}
	float distance = (GetActorLocation() - Targets[CurrentTargetNumber]).SizeSquared();

	if (distance < 10)
	{

		if (CurrentTargetNumber < 2)
		{
			CurrentTargetNumber++;
			Velocity = FVector(FMath::RandRange(-10, 10), FMath::RandRange(-10, 10), FMath::RandRange(-10, 10));
			MaxSpeed = Velocity.Size();

		}
		else
		{
			Velocity = FVector(0.0f);
		}


	}
	else {
		SetActorRotation(CalcAngles());
	}
}

FVector ABirdActor::CalcVelocity()
{
	FVector location = GetActorLocation();
	FVector desiredVelocity = (Targets[CurrentTargetNumber] - location);

	float distance = desiredVelocity.Size();

	desiredVelocity.GetSafeNormal(Velocity.Size());

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%.6f"),Velocity.Size()));*/
	desiredVelocity.Normalize(1); // safeNormal юзать вроде бы надо????? ну допустим

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%.6f"), Velocity.Size()));*/

	desiredVelocity *= Velocity.Size();
	FVector steering = (desiredVelocity - Velocity) * SteeringMagnitude;

	FVector result = Velocity + steering;
	result.Normalize(1);

	//return result*Velocity.Size();
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%.6f"), distance));*/
	return result * MaxSpeed * CalcArrival(distance);
}

float ABirdActor::CalcArrival(float distance) //todo implement smooth transition from low speed to high
{								//at departure
	float threshold = MaxSpeed * 50;
	float ratio = distance / threshold;
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%.6f"), res));*/
	if (distance < threshold)
		if (distance < 50)
	return PreviousRatio;
		else
		{
			PreviousRatio = ratio; // actually it feels REALLY awkward to store the prevoius one
			return ratio;
		}
	else
	{
		PreviousRatio = 1;
		return 1;
	}

}

void ABirdActor::PerformMovement()
{
	FVector NewLocation = GetActorLocation() + Velocity;
	SetActorLocation(NewLocation);
}

FRotator ABirdActor::CalcAngles()
{
	FRotator res = FRotator(0, 0, 0);
	////float pitchCosine = Velocity.X / Velocity.Size();
	////float rollCosine = Velocity.Y / Velocity.Size();
	////float yawCosine = Velocity.Z / Velocity.Size();
	//float pitchCosine = Velocity.X / Velocity.Size();
	//float rollCosine = Velocity.Z / Velocity.Size();
	//float yawCosine = Velocity.Y / Velocity.Size();
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%.6f"),
	//		UKismetMathLibrary::Acos(pitchCosine)));
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%.6f"),
	//		UKismetMathLibrary::Acos(rollCosine)));
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%.6f"),
	//		UKismetMathLibrary::Acos(yawCosine)));
	//return FRotator(UKismetMathLibrary::Acos(pitchCosine) * 180 / PI,
	//	UKismetMathLibrary::Acos(rollCosine) * 180 / PI,
	//	UKismetMathLibrary::Acos(yawCosine) * 180 / PI);

	//return Velocity.Rotation();
	return (Targets[CurrentTargetNumber] - GetActorLocation()).Rotation();
}

