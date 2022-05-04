// Code by Luc Cornu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SnowboardMovement.generated.h"

/**
 * 
 */
UCLASS()
class SHOTBOARD_API USnowboardMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()

public:
	// Sets default values for the movement's properties
	USnowboardMovement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
