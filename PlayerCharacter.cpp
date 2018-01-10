// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PlayerPawnMovementComponent.h"



// Sets default values
APlayerCharacter::APlayerCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	
	//setting the root component to a dummy scene
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Scene"));
	PlayerDefaultAnimation = ObjectInitializer.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("FlipbookComponent"));
	PlayerDefaultAnimation->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//sets up movement input bindings
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &APlayerCharacter::HorizontalMovement);

}

//horizontal movement of player character
void APlayerCharacter::HorizontalMovement(float Value) {
	//gets the direction of movement and moves the player that way
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	//now move in that direction
	AddMovementInput(Direction, Value);
	if (Value > 0 || Value < 0) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Moving."));
	}
	

}



