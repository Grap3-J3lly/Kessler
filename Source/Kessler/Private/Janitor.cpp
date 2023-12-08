// Fill out your copyright notice in the Description page of Project Settings.


#include "Janitor.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - -	//
//					PUBLIC LOGIC						//
// - - - - - - - - - - - - - - - - - - - - - - - - - -	//

AJanitor::AJanitor()
{

	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(100.f);
	Capsule->SetCapsuleRadius(50.f);
	SetRootComponent(Capsule);

	JanitorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("JanitorMesh"));
	JanitorMesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AJanitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DelayCharacterRotation(DeltaTime);
}

void AJanitor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJanitor::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJanitor::Look);

		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AJanitor::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AJanitor::Brake);
	}
	
}

// - - - - - - - - - - - - - - - - - - - - - - - - - -	//
//					PROTECTED LOGIC						//
// - - - - - - - - - - - - - - - - - - - - - - - - - -	//

void AJanitor::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(JanitorMappingContext, 0);
		}
	}

	PreviousRotation = GetActorRotation();
	OriginalDeceleration = Cast<UFloatingPawnMovement>(GetMovementComponent())->Deceleration;
}

void AJanitor::Move(const FInputActionValue& Value)
{
	const FVector DirectionAxisValue = Value.Get<FVector>();
	if (GetController())
	{
		if (DirectionAxisValue.X != 0.f)
		{
			AddMovementInput(GetActorForwardVector(), DirectionAxisValue.X, true);
		}
		if (DirectionAxisValue.Y != 0.f)
		{
			AddMovementInput(GetActorRightVector(), DirectionAxisValue.Y, true);
		}
		if (DirectionAxisValue.Z != 0.f)
		{
			AddMovementInput(GetActorUpVector(), DirectionAxisValue.Z, true);
		}
	}
}

void AJanitor::Look(const FInputActionValue& Value)
{
	const FVector LookAxisValue = Value.Get<FVector>();

	if (GetController())
	{
		// These should be mouse inputs
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);

		if (LookAxisValue.Z != 0.f)
		{
			AddControllerRollInput(LookAxisValue.Z);
		}
	}
}

void AJanitor::Brake(const FInputActionValue& Value)
{
	const bool IsBraking = Value.Get<bool>();
	UFloatingPawnMovement* FloatingPawnMovement = Cast<UFloatingPawnMovement>(GetMovementComponent());

	if (IsBraking)
	{
		FloatingPawnMovement->Deceleration = BrakeDeceleration;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Green, FString("Deceleration is On"));
		}
	}
	else
	{
		FloatingPawnMovement->Deceleration = OriginalDeceleration;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, FString("Deceleration is Off"));
		}
	}
}

void AJanitor::DelayCharacterRotation(float DeltaTime)
{
	FRotator CurrentRotation = GetControlRotation();
	if (PreviousRotation != CurrentRotation)
	{
		FRotator NewRotator = FMath::RInterpTo(PreviousRotation, CurrentRotation, DeltaTime, InterpSpeed);
		SetActorRotation(NewRotator);
		PreviousRotation = NewRotator;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - -	//
//					PRIVATE LOGIC						//
// - - - - - - - - - - - - - - - - - - - - - - - - - -	//