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
	bool bIsSlow = false;
	bool bIsStop = false;
	/** The angle of the slope */
	float Alpha;
	float Direction;
	float PreviousSpeed;
	float SpeedCoeff = 0.f;

protected:
	// Control variables :
	/** The jump force of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Controls")
		float JumpForce = 100.f;

	/** The maximum vector size of the velocity while the "slow button" is pressed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Controls")
		float MaxSlowSpeed = 750.f;

	/** The maximum turn in degrees the snowboard can make */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Controls")
		float MaxTurn = 55.f;

	/** The force used to rotate the actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Controls")
		float TurnForce = 550.f;


	/** If checked, show the debug lines (line trace) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
		bool bShowDebugLines = false;

	/** If true, the player is on the ground */
	UPROPERTY(BlueprintReadWrite, Category = "Character Movement")
		bool bIsOnGround;

	/** The length of the line trace for the ground detection */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utilities")
		float LineLength = 55.f;

	/** The velocity on the current frame */
	UPROPERTY(BlueprintReadWrite, Category = "Character Movement")
		FVector PreviousVelocity;

	/** Deprecated */
	UPROPERTY(BlueprintReadOnly, Category = "Utilities")
		FVector StartFront;

	/** Deprecated */
	UPROPERTY(BlueprintReadOnly, Category = "Utilities")
		FVector StartBack;

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
	// Controls :
	UFUNCTION(BlueprintCallable, Category = "Character Controls")
		void Jump();

	UFUNCTION(BlueprintCallable, Category = "Character Controls|Debug")
		void RestartLevel();

	UFUNCTION(BlueprintCallable, Category = "Character Controls")
		void RotateFlip(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Character Controls")
		void RotateRight(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Character Controls")
		void Stop();

	UFUNCTION(BlueprintCallable, Category = "Character Controls")
		void SlowDown();

	UFUNCTION(BlueprintCallable, Category = "Character Controls")
		void SpeedDown();

	UFUNCTION(BlueprintCallable, Category = "Character Controls")
		void SpeedUp();

	UFUNCTION(BlueprintCallable, Category = "Character Controls")
		void TurnRight(float AxisValue);

	// Others :
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void AlignBoard(const FVector A, const FVector B);

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void ComputeMovement(const float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void CheckGround();

	UFUNCTION(BlueprintCallable, Category = "Utilities")
		bool TraceLine(const FVector Start, const FVector End, FHitResult& OutHit);

public:
	UFUNCTION(BlueprintPure, Category = "Utilities")
		float GetAngleFromSin(const FVector A, const FVector B) const;

};
