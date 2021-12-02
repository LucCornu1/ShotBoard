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

protected:
	/** The variable for the speed at which the character builds momentum */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|BaseMovement", meta = (ClampMin = "0", ClampMax = "1"))
		float MomentumAlpha = 0.01;

	/** The variable for the speed at which the character turns */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|BaseMovement", meta = (ClampMin = "0", ClampMax = "1"))
		float TurnAlpha = 0.03;

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
		void MoveForward(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
		void TurnRight(float AxisValue);

public:

};
