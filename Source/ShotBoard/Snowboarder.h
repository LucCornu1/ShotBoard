// Code by Luc Cornu

#pragma once

#include "SnowboardMovementComponent.h"
#include "Components/SphereComponent.h"
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
	/** The length of the line trace for the ground detection */
	const float LineLength = 55.f;

protected:
	/** If checked, show the debug lines (line trace) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
		bool bShowDebugLines = false;

public:
	/** The Capsule component for the collision, and also the root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components|Root Component")
		USphereComponent* SphereComponent;

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
		USnowboardMovementComponent* SnowboardMovement;

// Functions
private:
	float GetSnowboardDirection(float A, float B);

protected:
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Snowboard")
		void AlignBoard(FVector A, FVector B, float Direction);

	UFUNCTION(BlueprintCallable, Category = "Character Movement|Snowboard")
		void CheckGround();

	UFUNCTION(BlueprintCallable, Category = "Utilities")
		bool TraceLine(FVector Start, FVector End, FHitResult& OutHit, bool bShowLines = false);

public:
	UFUNCTION(BlueprintPure, Category = "Utilities")
		float GetAngleFromSin(FVector A, FVector B);

};
