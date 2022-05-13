// Code by Luc Cornu


#include "Snowboarder.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASnowboarder::ASnowboarder()
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

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
	// SnowboardMovement->SetPawnOwner(this);

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

}

// Called to bind functionality to input	
void ASnowboarder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Added
float ASnowboarder::GetSnowboardDirection(const float A, const float B) const
{
	return (A < B) ? -1 : 1;

}

void ASnowboarder::AddGravity(const float DeltaTime)
{
	if (bIsOnGround)
		VectorGravity = FVector(0.f, 0.f, 0.f);
	VectorGravity += FVector(0.f, 0.f, Gravity * DeltaTime);
	VectorGravity.Z = FMath::Clamp(VectorGravity.Z, 0.f, 50.f);
	// AddActorLocalOffset(VectorGravity, true);
	AddMovementInput(VectorGravity, -1.f, true);

}

void ASnowboarder::AddSpeed(const float DeltaTime)
{
	if (bIsOnGround)
	{
		float Speed = Gravity * Alpha * DeltaTime;
		float MaxSpeed = Speed * 10;

		FVector NewVelocity = -MaxSpeed * GetActorForwardVector();
		Velocity = FMath::VInterpTo(Velocity, NewVelocity, GetWorld()->GetDeltaSeconds(), 0.4f);
		PreviousSpeed = Speed;
	}
	
}

void ASnowboarder::AlignBoard(const FVector A, const FVector B)
{
	FVector Hypothenuse = A - B;
	FVector C = FVector(A.X, A.Y, B.Z);
	FVector Oppose = C - A;
	Alpha = FMath::RadiansToDegrees(GetAngleFromSin(Oppose, Hypothenuse)) * Direction;

	FRotator Tilt = GetActorRotation();
	if (FMath::Abs(Tilt.Pitch - Alpha) > 1.f)
	{
		Tilt.Pitch = Alpha;
		Tilt = FMath::RInterpTo(GetRootComponent()->GetComponentRotation(), Tilt, GetWorld()->GetDeltaSeconds(), 10.f);
	}

	SetActorRelativeRotation(Tilt);

}

void ASnowboarder::ComputeMovement(const float DeltaTime)
{
	AddSpeed(DeltaTime);
	AddGravity(DeltaTime);
	AddMovementInput(Velocity);

	// if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Velocity %f, %f, %f"), GetVelocity().X , GetVelocity().Y, GetVelocity().Z));

}

void ASnowboarder::CheckGround()
{
	// Line trace settings
	FVector StartFront = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, 50.f, 0.f));
	FVector EndFront = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, 50.f, -LineLength));
	FHitResult OutHitFront;

	FVector StartCenter = SnowboardComponent->GetComponentLocation();
	FVector EndCenter = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, 0.f, -10.f));
	FHitResult OutHitCenter;
	
	FVector StartBack = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, -50.f, 0.f));
	FVector EndBack = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, -50.f, -LineLength));
	FHitResult OutHitBack;

	bool FrontHit = TraceLine(StartFront, EndFront, OutHitFront, bShowDebugLines);
	bool BackHit = TraceLine(StartBack, EndBack, OutHitBack, bShowDebugLines);
	bIsOnGround = TraceLine(StartCenter, EndCenter, OutHitCenter, bShowDebugLines);

	if (FrontHit && BackHit)
	{
		Direction = GetSnowboardDirection(OutHitFront.ImpactPoint.Z, OutHitBack.ImpactPoint.Z);
		AlignBoard(OutHitBack.ImpactPoint, OutHitFront.ImpactPoint);
	}

}

bool ASnowboarder::TraceLine(FVector Start, FVector End, FHitResult& OutHit, bool bShowLines)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (bShowLines) { DrawDebugLine(GetWorld(), Start, End, FColor::Red, false); } // If the debug variables is checked, show debug lines
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params)) // Trace a line from the snowboard to the ground under him
	{
		if (bShowLines) { DrawDebugPoint(GetWorld(), OutHit.ImpactPoint, 10, FColor::Green, true); }
		return true;
	}

	return false;

}

float ASnowboarder::GetAngleFromSin(const FVector A, const FVector B) const
{
	return FMath::Asin(A.Size() / B.Size());

}