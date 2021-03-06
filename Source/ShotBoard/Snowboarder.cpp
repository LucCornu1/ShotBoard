// Code by Luc Cornu


#include "Snowboarder.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASnowboarder::ASnowboarder()
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetSimulatePhysics(true);

	/*SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);*/

	SnowboardComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SnowboardMesh"));
	SnowboardComponent->SetupAttachment(RootComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
	this->AddOwnedComponent(PawnMovement);

}

// Called when the game starts or when spawned
void ASnowboarder::BeginPlay()
{
	Super::BeginPlay();

	Alpha = 0.f;
	Direction = 0.f;
	PreviousSpeed = 0.f;
	
}

// Called every frame
void ASnowboarder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckGround();
	ComputeMovement(DeltaTime);

	if (GEngine && bIsStop)
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Is Stopped")));

	/*if (bIsOnGround && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Is On Ground")));
		FVector F = GetActorForwardVector();
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Forward Vector %f, %f, %f"), F.X, F.Y, F.Z));
	}*/

}

// Called to bind functionality to input	
void ASnowboarder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASnowboarder::Jump);
	PlayerInputComponent->BindAction("RestartLevel", IE_Pressed, this, &ASnowboarder::RestartLevel);
	PlayerInputComponent->BindAxis("Flip", this, &ASnowboarder::RotateFlip);
	PlayerInputComponent->BindAxis("Rotate", this, &ASnowboarder::RotateRight);
	PlayerInputComponent->BindAction("SlowDown", IE_DoubleClick, this, &ASnowboarder::Stop);
	PlayerInputComponent->BindAction("SlowDown", IE_Pressed, this, &ASnowboarder::SlowDown);
	PlayerInputComponent->BindAction("SlowDown", IE_Released, this, &ASnowboarder::SpeedDown);
	PlayerInputComponent->BindAction("SpeedUp", IE_Released, this, &ASnowboarder::SpeedDown);
	PlayerInputComponent->BindAction("SpeedUp", IE_Pressed, this, &ASnowboarder::SpeedUp);
	PlayerInputComponent->BindAxis("Right", this, &ASnowboarder::TurnRight);

}

// Added
float ASnowboarder::GetSnowboardDirection(const float A, const float B) const
{
	return (A < B) ? -1 : 1;

}

void ASnowboarder::Jump()
{
	if (bIsOnGround)
	{
		FVector UpVector = GetActorUpVector() * JumpForce;
		FVector FrontVector = GetActorForwardVector() * JumpForce / 2;
		BoxComponent->AddImpulse(UpVector);
		BoxComponent->AddImpulse(FrontVector);
	}

}

void ASnowboarder::RestartLevel()
{
	FString MapName = GetWorld()->GetMapName();
	UGameplayStatics::OpenLevel(this, "Level00");

}

void ASnowboarder::RotateFlip(float AxisValue)
{
	if (!bIsOnGround)
	{
		FVector RotateForce = GetActorRightVector() * 5000 * -AxisValue;
		BoxComponent->AddTorqueInRadians(RotateForce);
	}

}

void ASnowboarder::RotateRight(float AxisValue)
{
	if (!bIsOnGround)
	{
		FVector RotateForce = GetActorForwardVector() * 5000 * AxisValue;
		BoxComponent->AddTorqueInRadians(RotateForce);
	}

}

void ASnowboarder::Stop()
{
	if (!bIsStop)
		bIsStop = true;
	else
		bIsStop = false;

}

void ASnowboarder::SlowDown()
{
	bIsSlow = true;

}

void ASnowboarder::SpeedDown()
{
	SpeedCoeff = 0.f;
	bIsSlow = false;

}

void ASnowboarder::SpeedUp()
{
	SpeedCoeff = 0.1f;

}

void ASnowboarder::TurnRight(float AxisValue)
{
	FVector Force = FVector(0.f, 1.f, 0.f) * AxisValue * TurnForce;

	BoxComponent->AddForceAtLocationLocal(Force, FVector(50.f, 0.f, 0.f));
	BoxComponent->AddForceAtLocationLocal(-Force, FVector(-50.f, 0.f, 0.f));

	FVector CurrentAngularVelocity = BoxComponent->GetPhysicsAngularVelocityInDegrees();
	FVector NextAngularVelocity = UKismetMathLibrary::ClampVectorSize(CurrentAngularVelocity, -MaxTurn, MaxTurn);
	BoxComponent->SetPhysicsAngularVelocityInDegrees(NextAngularVelocity);

}

void ASnowboarder::AlignBoard(const FVector A, const FVector B)
{
	FVector Hypothenuse = A - B;
	FVector C = FVector(A.X, A.Y, B.Z);
	FVector Oppose = C - A;
	Alpha = FMath::RadiansToDegrees(GetAngleFromSin(Oppose, Hypothenuse)) * Direction;

	// SetActorRelativeRotation(Tilt);

}

void ASnowboarder::ComputeMovement(const float DeltaTime)
{
	if (bIsOnGround)
	{
		FVector CurrentVelocity = BoxComponent->GetPhysicsLinearVelocity();
		FVector NormalizedVelocity = CurrentVelocity.GetSafeNormal();
		
		float DotProduct = FVector::DotProduct(NormalizedVelocity, GetActorForwardVector());
		float Speed = CurrentVelocity.Size() /*+ 2.5f*/ * DotProduct;
		
		if (bIsSlow)
			if (Speed > MaxSlowSpeed && Speed != 0)
				Speed = FMath::FInterpTo(Speed, MaxSlowSpeed, DeltaTime, 1.f);
			else if (Speed < -MaxSlowSpeed && Speed != 0)
				Speed = FMath::FInterpTo(Speed, -MaxSlowSpeed, DeltaTime, 1.f);
		if (bIsStop)
			Speed = FMath::FInterpTo(Speed, 0.f, DeltaTime, 5.f);

		FVector NextVelocity = Speed * GetActorForwardVector() * 1.0001;
		BoxComponent->SetPhysicsLinearVelocity(NextVelocity);

		// if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("NextVelocity %f, %f, %f"), NextVelocity.X, NextVelocity.Y, NextVelocity.Z));

		PreviousVelocity = NextVelocity;
	}

}

void ASnowboarder::CheckGround()
{
	// Line trace settings
	StartFront = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, 50.f, 0.f));
	FVector EndFront = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, 50.f, -LineLength));
	FHitResult OutHitFront;
	
	StartBack = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, -50.f, 0.f));
	FVector EndBack = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, -50.f, -LineLength));
	FHitResult OutHitBack;

	bool FrontHit = TraceLine(StartFront, EndFront, OutHitFront);
	bool BackHit = TraceLine(StartBack, EndBack, OutHitBack);
	bIsOnGround = FrontHit && BackHit;

	if (bIsOnGround)
	{
		Direction = GetSnowboardDirection(OutHitFront.ImpactPoint.Z, OutHitBack.ImpactPoint.Z);
		AlignBoard(OutHitBack.ImpactPoint, OutHitFront.ImpactPoint);
	}

}

bool ASnowboarder::TraceLine(const FVector Start, const FVector End, FHitResult& OutHit)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (bShowDebugLines) { DrawDebugLine(GetWorld(), Start, End, FColor::Red, false); } // If the debug variables is checked, show debug lines
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params)) // Trace a line from the snowboard to the ground under him
	{
		if (bShowDebugLines) { DrawDebugPoint(GetWorld(), OutHit.ImpactPoint, 10, FColor::Green, true); }
		return true;
	}

	return false;

}

float ASnowboarder::GetAngleFromSin(const FVector A, const FVector B) const
{
	return FMath::Asin(A.Size() / B.Size());

}
