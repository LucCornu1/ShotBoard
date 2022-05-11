// Code by Luc Cornu

#pragma once

#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Snowboarder.generated.h"

UCLASS()
class SHOTBOARD_API ASnowboarder : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnowboarder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Variables
private:
	/** The variable for the gravity */
	const float Gravity = 9.81f;

	/** The angle of the slope */
	float Alpha;

protected:
	/** If checked, show the debug lines (line trace) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
		bool bShowDebugLines = false;

	/** The length of the line trace for the ground detection */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utilities")
		float LineLength = 55.f;

	UPROPERTY(BlueprintReadWrite, Category = "Character Movement")
		FVector Velocity;

	UPROPERTY(BlueprintReadWrite, Category = "Character Movement")
		FVector PreviousVelocity;

public:
	/** The Capsule component for the collision, and also the root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components|Root Component")
		UBoxComponent* BoxComponent;

	/** The Skeletal Mesh component for the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		USkeletalMeshComponent* SkeletalMeshComponent;

	/** The Mesh component for the snowboard */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		UStaticMeshComponent* SnowboardComponent;

	/** The Arrow component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		UArrowComponent* ArrowComponent;

	/** The Spring Arm component attached to the player & the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		USpringArmComponent* SpringArmComponent;

	/** The Camera component attached to the spring arm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		UCameraComponent* CameraComponent;

	/** The Snowboard movement component of the snowboard */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		UFloatingPawnMovement* PawnMovement;

// Functions
private:
	float GetSnowboardDirection(const float A, const float B) const;

protected:
	UFUNCTION(BlueprintCallable, Category = "CharacterMovement")
		void AddGravity(const float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void AlignBoard(FVector A, FVector B, float Direction);

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void ComputeMovement(const float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void CheckGround();

	UFUNCTION(BlueprintCallable, Category = "Utilities")
		bool TraceLine(FVector Start, FVector End, FHitResult& OutHit, bool bShowLines = false);

public:
	UFUNCTION(BlueprintPure, Category = "Utilities")
		float GetAngleFromSin(FVector A, FVector B);

};
