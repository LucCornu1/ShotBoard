// Code by Luc Cornu & Gaëtan Piou.


#pragma once

#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shotboarder.generated.h"

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
	float AngleSpeed;
	UArrowComponent* ArrowComponent = GetArrowComponent();
	bool bForward;
	float ForwardMomentum;
	float PreviousSpeed;
	FVector DirectionMomentum;
	float AirMomentum;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		UStaticMeshComponent* SnowboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
		bool bShowDebugLines = false;


// Functions
private:
protected:
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void MoveForward(/*float AxisValue*/);

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void TurnRight(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
		void CameraTurn(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
		void CameraUp(float AxisValue);

	UFUNCTION(BlueprintPure, Category = "Character Movement|Speed")
		bool CheckUpdateSpeed(float BoardTilt) const;

	UFUNCTION(BlueprintPure, Category = "Character Movement|Speed")
		float GetAngleSpeed(const float Tilt) const;

	UFUNCTION(BlueprintPure, Category = "Character Movement|Speed")
		float GetAccelerationRate(const float Tilt) const;

	UFUNCTION(BlueprintCallable, Category = "Character Movement|Speed")
		void UpdateSpeed(float NewSpeed, float NewAccelerationRate, const FRotator Tilt);

	UFUNCTION(BlueprintCallable, Category = "Snowboard|Alignement")
		FRotator AlignBoard();

	UFUNCTION(BlueprintCallable, Category = "Snowboard|Alignements")
		void CheckGround();

public:
	UFUNCTION(BlueprintPure, Category = "Character Movement|Movement Mode")
		bool IsFlying() const { return GetCharacterMovement()->MovementMode == MOVE_Flying; }

};
