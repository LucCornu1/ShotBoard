// Code by Luc Cornu


#include "SnowboardMovementComponent.h"

// Sets default values
USnowboardMovementComponent::USnowboardMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned
void USnowboardMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void USnowboardMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ComputeMovement(DeltaTime);

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Hello There")));*/

}

// Added
void USnowboardMovementComponent::AddGravity(float DeltaTime)
{
	FVector VectorGravity(0.f, 0.f, Gravity * DeltaTime);
	VelocityGravity -= VectorGravity;
	PawnOwner->AddActorWorldOffset(VelocityGravity, true);
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Gravity %f"), Velocity.Z));*/
	
}

void USnowboardMovementComponent::ComputeMovement(float DeltaTime)
{
	if (PawnOwner)
	{
		AddGravity(DeltaTime);

		FVector NewVelocity;
		NewVelocity.X = FMath::Clamp(Velocity.X + Gravity * -AngleAlpha * DeltaTime, -100.f, 100.f);
		Velocity = FMath::VInterpTo(Velocity, NewVelocity, GetWorld()->GetDeltaSeconds(), 5.f);
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Velocity %f"), Velocity.X, Velocity.Y, Velocity.Z));*/
		PawnOwner->AddActorLocalOffset(Velocity, true);
	}

}