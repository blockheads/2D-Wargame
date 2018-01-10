// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Projectile.h"
#include "Engine.h"
#include "Engine/Blueprint.h"


// Sets default values
APlayerPawn::APlayerPawn(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;
	
	// Our root component will be a Box that reacts to physics
	BoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this,TEXT("BoxComponent"));	
	BoxComponent->SetCollisionProfileName(TEXT("Pawn"));
	BoxComponent->SetSimulatePhysics(false);
	
	RootComponent = BoxComponent;

	//creating and attaching the camera
	FirstPersonCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("CameraView"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);

	//creating and attaching the sprite flipbook
	PlayerFlipbook = ObjectInitializer.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("PlayerFlipbook"));
	PlayerFlipbook->AttachTo(RootComponent);

	HandFlipbook = ObjectInitializer.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("Hand"));
	HandFlipbook->AttachTo(PlayerFlipbook);

	OurMovementComponent = CreateDefaultSubobject<UPlayerPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
	
	
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	
	
	//hand that follows the mouse pointer
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	FVector MouseLocation, MouseDirection;

	Controller->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	
	//UE_LOG(LogTemp, Warning, TEXT("PlayerLocation: %s"), *GetActorLocation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("MouseLocation: %s"), *MouseLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("MouseDirection: %s"), *MouseDirection.ToString());

	
	//this ensures mouse is within the viewport
	if (MouseDirection.X) {
		//vertical aim
		float AimVert = MouseLocation.Z - GetActorLocation().Z;
		//horizontal aim
		float AimHorz = MouseLocation.X - GetActorLocation().X;

		//now taking hypotenuse of aimvery and aimhorz
		float AimHypoteneuse = FMath::Sqrt(FMath::Square(AimVert) + FMath::Square(AimHorz));

		float aimMax = 20.0f;
		//float actorHypot = FMath::Sqrt(FMath::Square(MouseLocation.Z + aimMax) + FMath::Square(MouseLocation.X + aimMax));


		if (AimHypoteneuse > aimMax || AimHypoteneuse < -aimMax) {
			float divideRatio = AimHypoteneuse / aimMax;
			float AimVertClamped = AimVert / divideRatio;
			float AimHorzClamped = AimHorz / divideRatio;

			MouseLocation.Z = GetActorLocation().Z + AimVertClamped;
			MouseLocation.X = GetActorLocation().X + AimHorzClamped;

			rifleLocX = AimHorzClamped;
			rifleLocZ = AimVertClamped;

		}
		else {
			float multiRatio = aimMax / AimHypoteneuse;
			rifleLocX = AimHorz*multiRatio;
			rifleLocZ = AimVert*multiRatio;
		}

		//this makes sure the gun doesn't get placed on the wrong y level
		MouseLocation.Y = GetActorLocation().Y;

		bool shouldRotateBullet = false;

		//rotate player
		if (AimHorz < 0) {
			shouldRotateBullet = true;
			//flipping the gun around
			AimHorz = -AimHorz;
			PlayerFlipbook->SetWorldRotation(FRotator(PlayerFlipbook->GetComponentRotation().Pitch, 180, PlayerFlipbook->GetComponentRotation().Roll));
		}
		else {
			PlayerFlipbook->SetWorldRotation(FRotator(PlayerFlipbook->GetComponentRotation().Pitch, 0, PlayerFlipbook->GetComponentRotation().Roll));
		}

		float rotation = FMath::RadiansToDegrees(FMath::Atan(AimVert / AimHorz));


		//UE_LOG(LogTemp, Warning, TEXT("Oppisite: %f"), AimVert);
		//UE_LOG(LogTemp, Warning, TEXT("Adjacent: %f"), AimHorz);
		//UE_LOG(LogTemp, Warning, TEXT("CalculatedRotation: %f"), rotation);
		HandFlipbook->SetWorldRotation(FRotator(rotation, HandFlipbook->GetComponentRotation().Yaw, HandFlipbook->GetComponentRotation().Roll));

		HandFlipbook->SetWorldLocation(MouseLocation);

		if (shouldRotateBullet) {
			shootDirection = FRotator(rotation, 180.0f, 0.0f);
		}
		else {
			shootDirection = FRotator(rotation, 0.0f, 0.0f);
		}
	}

	
	
	
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &APlayerPawn::HorizontalMovement);
	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &APlayerPawn::Jump);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerPawn::OnAttack);
	
}

//horizontal movement of player character
void APlayerPawn::HorizontalMovement(float Value) {
	//gets the direction of movement and moves the player that way
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	//now move in that direction
	AddMovementInput(Direction, Value);

}

void APlayerPawn::Jump() {
	//if can jump, jump
	FVector Direction = GetActorUpVector();
		
	OurMovementComponent->DoJump();
	
}

void APlayerPawn::OnAttack() {
	UWorld* World = GetWorld();
	if (World) {
		FTimerHandle AttackTimerHandle;
		World->GetTimerManager().SetTimer(AttackTimerHandle, this, &APlayerPawn::Attack, AttackDelay, false);
	}
}

void APlayerPawn::AttackFireDelay() {
	CanFire = true;
}

void APlayerPawn::Attack() {
	
	if (CanFire) {
		FVector location = HandFlipbook->GetComponentLocation() + FVector(rifleLocX, 0.0f, rifleLocZ);
		FRotator rotate = shootDirection + FRotator(0.0f, 0.0f, 0.0f);

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = Instigator;

		AProjectile* SpawnedBullet = GetWorld()->SpawnActor<AProjectile>(ProjectileToSpawn, location, rotate);
		if (SpawnedBullet) {

			SpawnedBullet->FireInDirection(shootDirection.Vector());
		}

		//sets up delay for next attack

		CanFire = false;

		UWorld* World = GetWorld();
		if (World) {
			FTimerHandle AttackFireTimerHandle;
			World->GetTimerManager().SetTimer(AttackFireTimerHandle, this, &APlayerPawn::AttackFireDelay, FireDelay, false);
		}
	}
	

}