// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawnMovementComponent.h"
#include "Engine.h"
#include "Engine/World.h"

void UPlayerPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//checking if everything is valid to move
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}

	//otherwise get and consume the movement vector from the pawn

	//velocity adds on the consumed input vector
	velocity += ConsumeInputVector();

	//gravity is a constant downward force
	velocity.Z -= gravity;
	if (velocity.Z <= terminalVelocity) {
		velocity.Z = terminalVelocity;
	}

	//then afterwards clamps it to a maximum of 1
	if (velocity.X > MaxVelocity) {
		velocity.X = MaxVelocity;
	}
	else if (velocity.X < -1) {
		velocity.X = -1;
	}
	
	FVector MovementVector = velocity * DeltaTime * 150.0f;

	//stop the character if velocity is terminal
	if (FMath::Abs(velocity.X) <= friction) {
		velocity.X = 0;
	}
	//otherwise decrements by the amount of friction
	//depending on if negative or positive
	else if (velocity.X > friction) {
		velocity -= FVector(friction, 0.0f, 0.0f);
	}
	else if(velocity.X < friction){
		velocity += FVector(friction, 0.0f, 0.0f);
	}
	
	
	if (!MovementVector.IsNearlyZero()) {
		FHitResult Hit;
	
		SafeMoveUpdatedComponent(MovementVector, UpdatedComponent->GetComponentRotation(), true, Hit);


		//if hit something slide along it
		if (Hit.IsValidBlockingHit()) {
			SlideAlongSurface(MovementVector, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
	

}

bool UPlayerPawnMovementComponent::DoJump() {
	
	FHitResult RV_Hit(ForceInit);
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")));
	//GetWorld()->DebugDrawTraceTag = "RV_Trace";

	DoLineTrace(&RV_Hit, &RV_TraceParams);

	if (RV_Hit.GetActor() == nullptr) {
		MovementState.bCanJump = false;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("hit %s"), *RV_Hit.GetActor()->GetName());
		MovementState.bCanJump = true;
	}

	if (MovementState.bCanJump) {
		velocity.Z = JumpZVelocity;
		MovementState.bCanJump = false;
		return true;
	}
	return false;

}

bool UPlayerPawnMovementComponent::DoLineTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams) {

	//start point of the line trace
	FVector Start = GetActorFeetLocation();
	//ends 10.0f below actor
	
	FVector End = GetActorFeetLocation() - FVector(0.0f,0.0f,2.0f);
	
	/*
	RV_TraceParams->bFindInitialOverlaps = false;
	RV_TraceParams->bTraceComplex = true;
	RV_TraceParams->bTraceAsyncScene = true;
	RV_TraceParams->bReturnPhysicalMaterial = true;
	*/
	
	
	
	//  do the line trace
	bool DidTrace = GetWorld()->LineTraceSingleByObjectType(
		*RV_Hit,        //result
		Start,        //start
		End,        //end
		ECC_WorldDynamic,    //collision channel
		*RV_TraceParams
		);


	return DidTrace;
		
}


