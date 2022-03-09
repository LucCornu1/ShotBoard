// Code by Luc Cornu


#pragma once

#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shotboarder.generated.h"

/**
The class for the player character of ShotBoard
*/
UCLASS()
class SHOTBOARD_API AShotboarder : public ACharacter
{
	GENERATED_BODY()

// Pre-generated
public:
	// Sets default values for this character's properties
	AShotboarder();

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
	float AirMomentum;
	float AngleSpeed;
	USkeletalMeshComponent* CharacterMeshComponent = GetMesh();
	// bool bCrouched;
	bool bForward;
	FVector DirectionMomentum;
	float ForwardMomentum;
	float PreviousSpeed;

protected:
	/** The variable for the speed at which the character turns */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|BaseMovement", meta = (ClampMin = "0", ClampMax = "1"))
		float TurnAlpha = 0.5f;

	/** The variable for the momentum */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|BaseMovement", meta = (ClampMin = "0", ClampMax = "1"))
		float MomentumAlpha = 0.01f;

	/** The curve that determine the speed corresponding to the slope */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Curves")
		UCurveFloat* SpeedCurve;

	/** The curve that determine the acceleration rate corresponding to the slope */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Curves")
		UCurveFloat* AccelerationRateCurve;

public:
	/** The Mesh component for the snowboard */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		UStaticMeshComponent* SnowboardComponent;

	/** The Spring Arm component attached to the player & the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		USpringArmComponent* SpringArmComponent;

	/** The Camera component attached to the spring arm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		UCameraComponent* CameraComponent;

	/** If checked, show the debug lines (line trace) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
		bool bShowDebugLines = false;


// Functions
private:

protected:
	/** Crouch the player if he's pressing the forward key (default Z) */
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void MoveForward(float AxisValue);

	/** Turn the character if he's pressing the right or left key (default Q & D) */
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void TurnRight(float AxisValue);

	/** Turn the camera according to the mouse X position (deactivated) */
	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
		void CameraTurn(float AxisValue);

	/** Turn the camera according to the mouse Y position (deactivated) */
	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
		void CameraUp(float AxisValue);

	/** Check if the player is eligible to update his speed */
	UFUNCTION(BlueprintPure, Category = "Character Movement|Speed")
		bool CheckUpdateSpeed(float BoardTilt) const;

	/** Get the speed based on the floor angle */
	UFUNCTION(BlueprintPure, Category = "Character Movement|Speed")
		float GetAngleSpeed(const float Tilt) const;

	/** Get the acceleration rate based on the floor angle */
	UFUNCTION(BlueprintPure, Category = "Character Movement|Speed")
		float GetAccelerationRate(const float Tilt) const;

	/** Update the speed of the player character based on the speed and acceleration rate retrieved before */
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Speed")
		void UpdateSpeed(float NewSpeed, float NewAccelerationRate, const FRotator Tilt);

	/** Return the tilt of the snowboard based on the floor tilt */
	UFUNCTION(BlueprintCallable, Category = "Snowboard|Alignement")
		FRotator AlignBoard();

	/** Check if the player is on the ground, and set the player movement mode according to the result */
	UFUNCTION(BlueprintCallable, Category = "Snowboard|Alignements")
		void CheckGround();

public:
	/** Return true if the player is flying */
	UFUNCTION(BlueprintPure, Category = "Character Movement|Movement Mode")
		bool IsFlying() const { return GetCharacterMovement()->MovementMode == MOVE_Flying; }

};
