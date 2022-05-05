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
void USnowboardMovementComponent::AddGravity(FVector VelocityVector, float DeltaTime)
{
	if (PawnOwner)
	{
		FVector VectorGravity(0.f, 0.f, Gravity * DeltaTime);
		Velocity -= VectorGravity;
		PawnOwner->AddActorWorldOffset(Velocity, true);
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Gravity %f"), Velocity.Z));*/
	}
	
}

void USnowboardMovementComponent::ComputeMovement(float DeltaTime)
{
	if (PawnOwner)
	{
		FVector VelocityVector = PawnOwner->GetActorLocation();
		AddGravity(VelocityVector, DeltaTime);
	}

}