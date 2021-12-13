// Code by Luc Cornu & Gaëtan Piou.


#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	PreviousLocation = GetActorLocation();

}

// Called every frame
void AShotboarder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator GroundTilt = AlignBoard();
	if (CheckUpdateSpeed())
	{
		UpdateSpeed(GetAngleSpeed(GroundTilt), GetAccelerationRate(GroundTilt), GroundTilt);

		PreviousLocation = GetActorLocation();
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
	GetCharacterMovement()->AddImpulse(SnowboardComponent->GetForwardVector() * 10000);

	// AddMovementInput(SnowboardComponent->GetForwardVector(), /*ForwardMomentum*/AxisValue);
	
}

void AShotboarder::TurnRight(float AxisValue)
{
	if (GetVelocity().Size() > 1)
	{
		AddActorLocalRotation(FRotator(0.f, 1000.f, 0.f) * AxisValue * TurnAlpha * GetWorld()->GetDeltaSeconds());
	}

}

void AShotboarder::CameraTurn(float AxisValue)
{
	AddActorLocalRotation(FRotator(0.f, 1000.f, 0.f) * AxisValue * TurnAlpha * GetWorld()->GetDeltaSeconds());
	// AddControllerYawInput(AxisValue);

}

void AShotboarder::CameraUp(float AxisValue)
{
	// AddControllerPitchInput(AxisValue);

}

bool AShotboarder::CheckUpdateSpeed() const
{
	if (IsValid(SpeedCurve) && IsValid(AccelerationRateCurve))
	{
		return (GetCharacterMovement()->GetLastUpdateVelocity().Size() > 0);
	}

	return false;

}

float AShotboarder::GetAngleSpeed(const FRotator Tilt) const
{
	return SpeedCurve->GetFloatValue(Tilt.Pitch);

}

float AShotboarder::GetAccelerationRate(const FRotator Tilt) const
{
	return AccelerationRateCurve->GetFloatValue(Tilt.Pitch);

}

void AShotboarder::UpdateSpeed(float NewSpeed, float NewAccelerationRate, const FRotator Tilt)
{
	if (GetCharacterMovement()->IsFalling())
	{
		NewAccelerationRate = 0.f;
	}

	NewSpeed = FMath::FInterpTo(PreviousSpeed, NewSpeed, GetWorld()->GetDeltaSeconds(), NewAccelerationRate);
	bool bTilt = Tilt == FRotator(0.f, 0.f, 0.f);
	/*if (NewSpeed < PreviousSpeed && NewSpeed < 0.2 && bTilt)
	{
		NewSpeed = 0.f;

		/*if (!bTilt)
		{
			NewSpeed = 0.0001f;
		}
	}*/

	AddMovementInput(SnowboardComponent->GetForwardVector(), NewSpeed, true);
	PreviousSpeed = NewSpeed;

}

FVector AShotboarder::LineTrace(FVector Start, FVector End)
{
	FHitResult OutHit;
	FCollisionQueryParams Params;

	if (bShowDebugLines) { DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f); }

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params) && OutHit.Actor != this)
	{
		// if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("true"))); }
		return OutHit.Location;
	}

	return End;

}

FRotator AShotboarder::AlignBoard()
{
	FVector Start = GetActorLocation() - GetActorRotation().Vector() * 70 + FVector(0.f, 0.f, -50.f);
	FVector End = Start + FVector(0.f, 0.f, -80.f);
	FVector BackHit = LineTrace(Start, End);

	Start = GetActorLocation() + GetActorRotation().Vector() * 70 + FVector(0.f, 0.f, -50.f);
	End = Start + FVector(0.f, 0.f, -80.f);
	FVector FrontHit = LineTrace(Start, End);

	FRotator Tilt = UKismetMathLibrary::FindLookAtRotation(BackHit, FrontHit);
	SnowboardComponent->SetWorldRotation(FMath::Lerp(SnowboardComponent->GetComponentRotation(), Tilt, 0.2f));
	return Tilt;

}
