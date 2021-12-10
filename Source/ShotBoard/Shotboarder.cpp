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

	SkateboardComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SnowboardMesh"));
	SkateboardComponent->SetupAttachment(RootComponent);

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
		UpdateSpeed(GetAngleSpeed(GroundTilt), GetAccelerationRate(GroundTilt));

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
	GetCharacterMovement()->AddImpulse(SkateboardComponent->GetForwardVector() * 10000);

	// AddMovementInput(SkateboardComponent->GetForwardVector(), /*ForwardMomentum*/AxisValue);
	
}

void AShotboarder::TurnRight(float AxisValue)
{
	// AxisValue != 0.f ? MA = 0 : MA = MomentumAlpha;

	// (SkateboardComponent->GetRightVector(), AxisValue * TurnAlpha
	AddActorLocalRotation(FRotator(0.f, 1.f, 0.f) * AxisValue /** TurnAlpha*/);

}

void AShotboarder::CameraTurn(float AxisValue)
{
	AddControllerYawInput(AxisValue);

}

void AShotboarder::CameraUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);

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

void AShotboarder::UpdateSpeed(float NewSpeed, float NewAccelerationRate)
{
	NewSpeed = FMath::FInterpTo(PreviousSpeed, NewSpeed, GetWorld()->GetDeltaSeconds(), NewAccelerationRate);

	if (NewSpeed < PreviousSpeed && NewSpeed < 0.2)
	{
		NewSpeed = 0.f;
	}

	FVector ForwardVector = (GetActorLocation() - PreviousLocation) /*FVector((GetActorLocation() - PreviousLocation).X, (GetActorLocation() - PreviousLocation).Y, 0.f)*/;
	ForwardVector.Normalize();

	AddMovementInput(SkateboardComponent->GetForwardVector(), NewSpeed);
	PreviousSpeed = NewSpeed;

}

FVector AShotboarder::LineTrace(FVector Start, FVector End)
{
	FHitResult OutHit;
	FCollisionQueryParams Params;

	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
	// if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("%s"), bHit ? TEXT("true") : TEXT("false"))); }

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params) && OutHit.Actor != this)
	{
		// if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("true"))); }
		return OutHit.Location;
	}

	return End;

}

FRotator AShotboarder::AlignBoard()
{
	FVector Start = GetActorLocation() - GetActorRotation().Vector() * 70 + FVector(0.f, 0.f, -60.f);
	FVector End = Start + FVector(0.f, 0.f, -70.f);
	FVector BackHit = LineTrace(Start, End);

	Start = GetActorLocation() + GetActorRotation().Vector() * 70 + FVector(0.f, 0.f, -60.f);
	End = Start + FVector(0.f, 0.f, -70.f);
	FVector FrontHit = LineTrace(Start, End);

	FRotator Tilt = UKismetMathLibrary::FindLookAtRotation(BackHit, FrontHit);
	SkateboardComponent->SetWorldRotation(Tilt);
	return Tilt;

}
