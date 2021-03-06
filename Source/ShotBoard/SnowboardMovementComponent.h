// Code by Luc Cornu

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SnowboardMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOTBOARD_API USnowboardMovementComponent : public UActorComponent
{
	GENERATED_BODY()

// Pre-generated
public:
	// Sets default values for the movement's properties
	USnowboardMovementComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Variables
private:
	/** The Angle of the snowboard */
	float AngleAlpha;

	/** The variable for the gravity */
	const float Gravity = 9.81f;

protected:

	/** Pawn that owns this component. */
	class APawn* PawnOwner;

	UPROPERTY(BlueprintReadWrite, Category = "Character Movement")
		FVector Velocity;

	UPROPERTY(BlueprintReadWrite, Category = "Character Movement")
		FVector VelocityGravity;

public:

// Functions
private:

protected:
	/** Add gravity to the velocity */
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Gravity")
		void AddGravity(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void ComputeMovement(float DeltaTime);

public:
	UFUNCTION(BlueprintCallable, Category = "Setter")
		void SetAngleAlpha(float Angle) { AngleAlpha = Angle; };

	UFUNCTION(BlueprintCallable, Category = "Setter")
		void SetPawnOwner(class APawn* Owner) { PawnOwner = Owner; };
	
};
