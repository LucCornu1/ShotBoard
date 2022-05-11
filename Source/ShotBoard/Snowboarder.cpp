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
	FVector VectorGravity(0.f, 0.f, Gravity * DeltaTime);
	Velocity -= VectorGravity;
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Gravity %f"), Velocity.Z));*/

}

void ASnowboarder::AlignBoard(FVector A, FVector B, float Direction)
{
	FVector Hypothenuse = A - B;
	FVector C = FVector(A.X, A.Y, B.Z);
	FVector Oppose = C - A;
	Alpha = FMath::RadiansToDegrees(GetAngleFromSin(Oppose, Hypothenuse)) * Direction;

	FRotator Tilt = GetActorRotation();
	Tilt.Pitch = Alpha;
	Tilt = FMath::RInterpTo(GetRootComponent()->GetComponentRotation(), Tilt, GetWorld()->GetDeltaSeconds(), 10.f);

	SetActorRotation(Tilt);

}

void ASnowboarder::ComputeMovement(const float DeltaTime)
{
	AddGravity(DeltaTime);

	FVector NewVelocity;
	NewVelocity = (Velocity.X + Gravity * Alpha * DeltaTime) * GetActorForwardVector();
	Velocity = FMath::VInterpTo(PreviousVelocity, NewVelocity, GetWorld()->GetDeltaSeconds(), 0.5f);
	PreviousVelocity = -NewVelocity;
	AddMovementInput(Velocity);

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Velocity %f"), Velocity.X, Velocity.Y, Velocity.Z));*/

}

void ASnowboarder::CheckGround()
{
	// Line trace settings
	FVector StartFront = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, 62.f, LineLength));
	FVector EndFront = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, 62.f, -LineLength));
	FVector StartBack = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, -62.f, LineLength));
	FVector EndBack = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, -62.f, -LineLength));
	FHitResult OutHitFront;
	FHitResult OutHitBack;

	bool FrontHit = TraceLine(StartFront, EndFront, OutHitFront, bShowDebugLines);
	bool BackHit = TraceLine(StartBack, EndBack, OutHitBack, bShowDebugLines);

	if (FrontHit && BackHit)
	{
		float Direction = GetSnowboardDirection(OutHitFront.ImpactPoint.Z, OutHitBack.ImpactPoint.Z);
		AlignBoard(OutHitBack.ImpactPoint, OutHitFront.ImpactPoint, Direction);
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

float ASnowboarder::GetAngleFromSin(FVector A, FVector B)
{
	return FMath::Asin(A.Size() / B.Size());

}