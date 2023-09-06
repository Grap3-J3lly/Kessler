// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "BasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class KESSLER_API ABasePlayer : public ABasePawn
{
	GENERATED_BODY()
	

public:
	ABasePlayer();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float BoostAmount = 10000;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float RotationRate = 10;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
		float Speed = 1000;


	void BoostForward(float AxisValue);
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
};
