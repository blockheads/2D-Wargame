// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PlayerPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MUTIPLAYERKNIGHTS_API UPlayerPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	bool DoJump();

	bool DoLineTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams);

	
	
protected:
	//this determines the pawn's current velocity at the moment
	FVector velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = movement)
	float JumpZVelocity = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = movement)
	float friction = .05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = movement)
	float gravity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = movement)
	float terminalVelocity = -3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = movement)
	float MaxVelocity = 1.0f;
	
};
