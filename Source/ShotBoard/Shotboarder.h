// Code by Luc Cornu & Gaëtan Piou.


#pragma once

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
	float ForwardMomentum;
	FVector PreviousLocation;
	float AngleSpeed;
	bool bForward;

protected:
	/** The variable for the speed at which the character turns */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|BaseMovement", meta = (ClampMin = "0", ClampMax = "1"))
		float TurnAlpha = 0.04f;

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
		UStaticMeshComponent* SkateboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
		UCameraComponent* CameraComponent;


// Functions
private:
protected:
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void IsForward();

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void NotForward();

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void MoveForward(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void TurnRight(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
		void CameraTurn(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
		void CameraUp(float AxisValue);

	UFUNCTION(BlueprintPure, Category = "Character Movement|Speed")
		bool CheckUpdateSpeed();

	UFUNCTION(BlueprintPure, Category = "Character Movement|Speed")
		float GetAngleSpeed();

	UFUNCTION(BlueprintPure, Category = "Character Movement|Speed")
		float GetAccelerationRate();

	UFUNCTION(BlueprintCallable, Category = "Character Movement|Speed")
		void UpdateSpeed(float NewSpeed, float NewAccelerationRate);

public:

};
