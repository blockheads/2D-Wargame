// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawnMovementComponent.h"
#include "PlayerPawn.generated.h"

UCLASS()
class MUTIPLAYERKNIGHTS_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbookComponent* PlayerFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbookComponent* HandFlipbook;

	//bullet to spawn
	UPROPERTY(EditAnywhere, Category = Spawning)
	TSubclassOf<class AProjectile> ProjectileToSpawn;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CollisionBox)
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FirstPersonCameraComponent;
	
	//function for horizontal player movement
	UFUNCTION()
	void HorizontalMovement(float Value);
	
	//function for attacking
	UFUNCTION()
	void Attack();

	//Attack Delay
	UFUNCTION()
	void OnAttack();

	//post attack delay
	UFUNCTION()
		void AttackFireDelay();

	//function for jumping
	UFUNCTION()
	void Jump();

	
	bool CanFire = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementComponent)
	class UPlayerPawnMovementComponent* OurMovementComponent;

	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackDelay = 0.1f;

	UPROPERTY(EditAnywhere, Category = Attack)
	float FireDelay = 1.0f;

	//floats that store the current rifle offset in the y and x direction
	float rifleLocX,rifleLocZ;
	//stores the angle to shoot at
	FRotator shootDirection;
};
