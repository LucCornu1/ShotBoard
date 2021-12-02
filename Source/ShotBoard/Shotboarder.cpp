// Code by Luc Cornu & Gaëtan Piou.


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

}

// Called to bind functionality to input
void AShotboarder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &AShotboarder::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &AShotboarder::TurnRight);

}


// Added
void AShotboarder::MoveForward(float AxisValue)
{
	ForwardMomentum = FMath::Lerp(ForwardMomentum, AxisValue, MomentumAlpha);

	AddMovementInput(SkateboardComponent->GetForwardVector() , ForwardMomentum);
	
}

void AShotboarder::TurnRight(float AxisValue)
{
	AddMovementInput(SkateboardComponent->GetRightVector(), AxisValue * TurnAlpha);

}