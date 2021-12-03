// Code by Luc Cornu & Gaëtan Piou.


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
}

// Called every frame
void AShotboarder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckUpdateSpeed())
	{
		UpdateSpeed(GetAngleSpeed(), GetAccelerationRate());
		PreviousLocation = GetActorLocation();

	}

}

// Called to bind functionality to input
void AShotboarder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Forward", IE_Pressed, this, &AShotboarder::IsForward);
	PlayerInputComponent->BindAction("Forward", IE_Released, this, &AShotboarder::NotForward);

	PlayerInputComponent->BindAxis("Forward", this, &AShotboarder::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &AShotboarder::TurnRight);

	PlayerInputComponent->BindAxis("Turn", this, &AShotboarder::CameraTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShotboarder::CameraUp);

}


// Added
void AShotboarder::IsForward()
{
	bForward = true;

}

void AShotboarder::NotForward()
{
	bForward = false;

}

void AShotboarder::MoveForward(float AxisValue)
{
	// ForwardMomentum = FMath::Lerp(ForwardMomentum, AxisValue, MomentumAlpha);

	AddMovementInput(SkateboardComponent->GetForwardVector(), /*ForwardMomentum*/AxisValue);
	
}

void AShotboarder::TurnRight(float AxisValue)
{
	// AxisValue != 0.f ? MA = 0 : MA = MomentumAlpha;

	AddMovementInput(SkateboardComponent->GetRightVector(), AxisValue * TurnAlpha);

}

void AShotboarder::CameraTurn(float AxisValue)
{
	AddControllerYawInput(AxisValue);

}

void AShotboarder::CameraUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);

}

bool AShotboarder::CheckUpdateSpeed()
{
	if (IsValid(SpeedCurve) && IsValid(AccelerationRateCurve))
	{
		return (/*(!GetCharacterMovement()->IsFalling()) &&*/ (GetCharacterMovement()->GetLastUpdateVelocity().Size() > 0));
	}

	return false;

}

float AShotboarder::GetAngleSpeed()
{
	return SpeedCurve->GetFloatValue(UKismetMathLibrary::FindLookAtRotation(PreviousLocation, GetActorLocation()).Pitch);

}

float AShotboarder::GetAccelerationRate()
{
	return AccelerationRateCurve->GetFloatValue(UKismetMathLibrary::FindLookAtRotation(PreviousLocation, GetActorLocation()).Pitch);

}

void AShotboarder::UpdateSpeed(float NewSpeed, float NewAccelerationRate)
{
	GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, NewSpeed, GetWorld()->GetDeltaSeconds(), NewAccelerationRate);

}
