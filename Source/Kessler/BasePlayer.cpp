// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"

ABasePlayer::ABasePlayer()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAxis(TEXT("BoostForward"), this, &ABasePlayer::BoostForward);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABasePlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABasePlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ABasePlayer::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &ABasePlayer::LookRightRate);
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ConsumeMovementInputVector();
}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePlayer::BoostForward(float AxisValue)
{
	/*AddMovementInput(GetActorForwardVector() * BoostAmount * Speed);
	*/
	if (AxisValue > 0)
	{
		FloatingMovementComponent->Velocity += GetActorForwardVector() * BoostAmount * AxisValue;
		FloatingMovementComponent->UpdateComponentVelocity();
	}
}

void ABasePlayer::MoveForward(float AxisValue)
{
	//FVector DeltaLocation = FVector::ZeroVector;
	//// X = Value * DeltaTime * Speed
	//DeltaLocation.X = AxisValue * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);

	//AddActorLocalOffset(DeltaLocation, true);

	if (AxisValue != 0)
	{
		FloatingMovementComponent->Velocity += GetActorForwardVector() * Speed * AxisValue;
		FloatingMovementComponent->UpdateComponentVelocity();
	}
	
}

void ABasePlayer::MoveRight(float AxisValue)
{
	//FVector DeltaLocation = FVector::ZeroVector;
	//// X = Value * DeltaTime * Speed
	//DeltaLocation.Y = AxisValue * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);

	//AddActorLocalOffset(DeltaLocation, true);

	if (AxisValue != 0)
	{
		FloatingMovementComponent->Velocity += GetActorRightVector() * Speed * AxisValue;
		FloatingMovementComponent->UpdateComponentVelocity();
	}
}

void ABasePlayer::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ABasePlayer::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

