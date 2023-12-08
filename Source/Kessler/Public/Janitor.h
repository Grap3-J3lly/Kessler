// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Janitor.generated.h"

// Forward Declarations
class UCameraComponent;
class UCapsuleComponent;
class UInputAction;
class UInputMappingContext;
class USkeletalMeshComponent;
class USpringArmComponent;

UCLASS()
class KESSLER_API AJanitor : public APawn
{
	GENERATED_BODY()

public:
	AJanitor();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* JanitorMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* BrakeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	float InterpSpeed = .5f;

	UPROPERTY(EditAnywhere, Category = "Input")
	float BrakeDeceleration = .5f;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Brake(const FInputActionValue& Value);


private:
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* JanitorMesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	FRotator PreviousRotation;

	UPROPERTY()
	float OriginalDeceleration = 0.f;

	UFUNCTION()
	void DelayCharacterRotation(float DeltaTime);

};
