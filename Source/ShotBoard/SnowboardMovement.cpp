// Code by Luc Cornu


#include "SnowboardMovement.h"

// Sets default values
USnowboardMovement::USnowboardMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned
void USnowboardMovement::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void USnowboardMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Hello There")));

}