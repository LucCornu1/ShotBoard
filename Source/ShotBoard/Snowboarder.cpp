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

	SnowboardMovement = CreateDefaultSubobject<USnowboardMovementComponent>(TEXT("SnowboardMovement"));
	this->AddOwnedComponent(SnowboardMovement);
	SnowboardMovement->SetPawnOwner(this);

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

}

// Called to bind functionality to input	
void ASnowboarder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int ASnowboarder::GetSnowboardDirection(FVector A, FVector B)
{
	if (A.Size() > B.Size())
	{
		return -1;
	}

	return 1;

}

void ASnowboarder::AlignBoard(FVector A, FVector B)
{
	FVector Hypothenuse = A - B;
	FVector C = FVector(A.X, A.Y, B.Z);
	FVector Oppose = C - A;
	float Angle = FMath::RadiansToDegrees(GetAngleSin(Oppose, Hypothenuse));

	float Direction = GetSnowboardDirection(A, B);
	FRotator Tilt = GetActorRotation();
	Tilt.Pitch = Angle * Direction;
	Tilt = FMath::RInterpTo(GetRootComponent()->GetComponentRotation(), Tilt, GetWorld()->GetDeltaSeconds(), 15.f);

	SetActorRotation(Tilt);

}

// Added
void ASnowboarder::CheckGround()
{
	// Line trace settings
	FVector StartFront = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, 62.f, LineLength));
	FVector EndFront = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, 62.f, -LineLength));
	FVector StartBack = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, -62.f, LineLength));
	FVector EndBack = UKismetMathLibrary::TransformLocation(SnowboardComponent->GetComponentTransform(), FVector(0.f, -62.f, -LineLength));
	FHitResult OutHitFront;
	FHitResult OutHitBack;

	bool FrontHit = TraceLine(StartFront, EndFront, OutHitFront);
	bool BackHit = TraceLine(StartBack, EndBack, OutHitBack);

	if (FrontHit || BackHit)
	{
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

float ASnowboarder::GetAngleSin(FVector A, FVector B)
{
	return FMath::Asin(A.Size() / B.Size());

}