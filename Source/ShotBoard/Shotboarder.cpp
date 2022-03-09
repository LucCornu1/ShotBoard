// Code by Luc Cornu


#include "Shotboarder.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AShotboarder::AShotboarder()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicates(true);
	SetReplicateMovement(true);
	
	SnowboardComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SnowboardMesh"));
	SnowboardComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

}

// Called when the game starts or when spawned
void AShotboarder::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShotboarder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator GroundTilt = AlignBoard();
	if (CheckUpdateSpeed(GroundTilt.Pitch) || IsFlying())
	{
		UpdateSpeed(GetAngleSpeed(GroundTilt.Pitch), GetAccelerationRate(GroundTilt.Pitch), GroundTilt);
		CheckGround();
	}

}

// Called to bind functionality to input
void AShotboarder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &AShotboarder::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &AShotboarder::TurnRight);
	PlayerInputComponent->BindAxis("Turn", this, &AShotboarder::CameraTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShotboarder::CameraUp);

}


// Added
void AShotboarder::MoveForward(float AxisValue)
// Goal : Make the player crouch if forward key is pressed
{
	if (AxisValue > 0)
	{
		Crouch(true);
	}
	else {
		UnCrouch(true);
	}
	
}

void AShotboarder::TurnRight(float AxisValue)
// Goal : Make the player turn if the right key or left key is pressed
{
	AddActorLocalRotation(FRotator(0.f, 1000.f, 0.f) * AxisValue * TurnAlpha * GetWorld()->GetDeltaSeconds()); // Turn the player character

}

void AShotboarder::CameraTurn(float AxisValue)
// Goal : Turn the camera according to the mouse X
//		-- [Deactivated]
{
	// AddActorLocalRotation(FRotator(0.f, 1000.f, 0.f) * AxisValue * TurnAlpha * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(AxisValue);

}

void AShotboarder::CameraUp(float AxisValue)
// Goal : Turn the camera according to the mouse Y
//		-- [Deactivated]
{
	AddControllerPitchInput(AxisValue);

}

bool AShotboarder::CheckUpdateSpeed(float BoardTilt) const
// Goal : Return true if the player snowbaord tilt is not equal to 0
{
	if (IsValid(SpeedCurve) && IsValid(AccelerationRateCurve))
	{
		return (BoardTilt != 0.f);
	}

	return false;

}

float AShotboarder::GetAngleSpeed(const float Tilt) const
// Goal : Return the speed based on the floor angle
{
	return SpeedCurve->GetFloatValue(Tilt);

}

float AShotboarder::GetAccelerationRate(const float Tilt) const
// Goal : Return the acceleration rate based on the floor angle
{
	return AccelerationRateCurve->GetFloatValue(Tilt);

}

void AShotboarder::UpdateSpeed(float NewSpeed, float NewAccelerationRate, const FRotator Tilt)
// Goal : Update the player speed based on the speed, the acceleration rate and the player movement mode
{
	if (IsFlying()) // If player is flying, apply corresponding momentum
	{
		AddMovementInput(DirectionMomentum, AirMomentum, false); // Apply the Momentum in the right direction
		// AddMovementInput(FVector(0.f, 0.f, -1.f), 10.f, true);

		DirectionMomentum -= FVector(0.f, 0.f, GetWorld()->GetDeltaSeconds()); // Apply gravity to make the player fall
		DirectionMomentum.Normalize(1.f);
	}
	else { // Else apply normal movement
		NewSpeed = FMath::FInterpTo(PreviousSpeed, NewSpeed, GetWorld()->GetDeltaSeconds(), NewAccelerationRate); // Interp the new speed with the previous one based on the acceleration of the character
		// --> Used to apply a slow acceleration at the start, which then increases
		bool bTilt = Tilt == FRotator(0.f, 0.f, 0.f);

		AddMovementInput(GetActorForwardVector(), NewSpeed, true); // Apply the speed in the direction of the slope
		PreviousSpeed = NewSpeed; // Store the previous speed
	}

}

FRotator AShotboarder::AlignBoard()
// Goal : Return the snowboard tilt based on the floor angle below the snowboard
//		-- [Bug] : The snowboard is too sensitive to the ground tilt
{
	// Line trace settings
	FVector Start = CharacterMeshComponent->GetComponentLocation();
	FVector End = UKismetMathLibrary::TransformLocation(GetActorTransform(),FVector(0.f, 0.f, -150.f));
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FRotator Tilt = FRotator(0.f, 0.f, 0.f);

	if (bShowDebugLines) { DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f); } // If the debug variables is checked, show debug lines
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params)) // Trace a line from center of player character to the ground under him
	{
		FVector UpVector = GetRootComponent()->GetUpVector();
		FVector NormalVector = OutHit.ImpactNormal;

		FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
		RotationAxis.Normalize();

		float DotProduct = FVector::DotProduct(UpVector, NormalVector);
		float RotationAngle = acosf(DotProduct);

		FQuat Quat = FQuat(RotationAxis, RotationAngle);
		FQuat RootQuat = GetRootComponent()->GetComponentQuat();

		FQuat NewQuat = Quat * RootQuat;
		Tilt = NewQuat.Rotator();

		Tilt = FMath::RInterpTo(GetRootComponent()->GetComponentRotation(), Tilt, GetWorld()->GetDeltaSeconds(), 15.f);

		GetRootComponent()->SetWorldRotation(Tilt);
	}

	return Tilt;

}

void AShotboarder::CheckGround()
// Goal : Check if the player is on the ground or flying, and set the corresponding movement mode
{
	// Line trace settings
	FVector Start = CharacterMeshComponent->GetComponentLocation();
	FVector End = UKismetMathLibrary::TransformLocation(GetActorTransform(), FVector(125.f, 0.f, -125.f));
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (bShowDebugLines) { DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f); } // If the debug variables is checked, show debug lines
	if (!IsFlying()) // If the player character is not flying
	{
		if (!GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params)) // Line trace not colliding with ground
		{
			GetCharacterMovement()->MovementMode = MOVE_Flying; // Set "Flying"
			DirectionMomentum = GetVelocity();
			DirectionMomentum.Normalize(1.f);
			AirMomentum = GetVelocity().Size(); // Set the AirMomentum & DirectionMomentum to keep the player moving forward after taking off the ground
		}
	}
	else { // If the player character is flying
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params)) // Line trace colliding with ground
		{
			GetCharacterMovement()->MovementMode = MOVE_Walking; // Set "Walking"
		}
	}
	
}
