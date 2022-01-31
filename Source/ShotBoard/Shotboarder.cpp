// Code by Luc Cornu & Gaëtan Piou.


#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Shotboarder.h"

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

	PlayerInputComponent->BindAction("Forward", IE_Pressed, this, &AShotboarder::MoveForward);

	// PlayerInputComponent->BindAxis("Forward", this, &AShotboarder::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &AShotboarder::TurnRight);

	PlayerInputComponent->BindAxis("Turn", this, &AShotboarder::CameraTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShotboarder::CameraUp);

}


// Added
void AShotboarder::MoveForward(/*float AxisValue*/)
{
	// GetCharacterMovement()->AddImpulse(SnowboardComponent->GetForwardVector() * 10000);

	// AddMovementInput(SnowboardComponent->GetForwardVector(), /*ForwardMomentum*/AxisValue);
	
}

void AShotboarder::TurnRight(float AxisValue)
{
	AddActorLocalRotation(FRotator(0.f, 1000.f, 0.f) * AxisValue * TurnAlpha * GetWorld()->GetDeltaSeconds());

}

void AShotboarder::CameraTurn(float AxisValue)
{
	// AddActorLocalRotation(FRotator(0.f, 1000.f, 0.f) * AxisValue * TurnAlpha * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(AxisValue);

}

void AShotboarder::CameraUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);

}

bool AShotboarder::CheckUpdateSpeed(float BoardTilt) const
{
	if (IsValid(SpeedCurve) && IsValid(AccelerationRateCurve))
	{
		return (BoardTilt != 0.f);
	}

	return false;

}

float AShotboarder::GetAngleSpeed(const float Tilt) const
{
	return SpeedCurve->GetFloatValue(Tilt);

}

float AShotboarder::GetAccelerationRate(const float Tilt) const
{
	return AccelerationRateCurve->GetFloatValue(Tilt);

}

void AShotboarder::UpdateSpeed(float NewSpeed, float NewAccelerationRate, const FRotator Tilt)
{
	if (IsFlying())
	{
		AddMovementInput(DirectionMomentum, AirMomentum, true);
		// AddMovementInput(FVector(0.f, 0.f, -1.f), 10.f, true);

		DirectionMomentum -= FVector(0.f, 0.f, GetWorld()->GetDeltaSeconds());
		DirectionMomentum.Normalize(1.f);
	}
	else {
		NewSpeed = FMath::FInterpTo(PreviousSpeed, NewSpeed, GetWorld()->GetDeltaSeconds(), NewAccelerationRate);
		bool bTilt = Tilt == FRotator(0.f, 0.f, 0.f);

		AddMovementInput(GetActorForwardVector(), NewSpeed, true);
		PreviousSpeed = NewSpeed;
	}

}

FRotator AShotboarder::AlignBoard()
{
	FVector Start = ArrowComponent->GetComponentLocation();
	FVector End = UKismetMathLibrary::TransformLocation(GetActorTransform(),FVector(0.f, 0.f, -150.f));
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FRotator Tilt = FRotator(0.f, 0.f, 0.f);

	if (bShowDebugLines) { DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f); }
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params))
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
{
	FVector Start = ArrowComponent->GetComponentLocation();
	FVector End = UKismetMathLibrary::TransformLocation(GetActorTransform(), FVector(125.f, 0.f, -125.f));
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (bShowDebugLines) { DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f); }
	if (!IsFlying())
	{
		if (!GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params))
		{
			GetCharacterMovement()->MovementMode = MOVE_Flying;
			DirectionMomentum = GetVelocity();
			DirectionMomentum.Normalize(1.f);
			AirMomentum = GetVelocity().Size();
		}
	}
	else {
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params))
		{
			GetCharacterMovement()->MovementMode = MOVE_Walking;
		}
	}
	
}
