// Fill out your copyright notice in the Description page of Project Settings.

#include "BreakableTile.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
ABreakableTile::ABreakableTile(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Our root component will be a Box that reacts to physics
	BoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxComponent"));
	//BoxComponent->SetCollisionProfileName(TEXT("Tile"));
	BoxComponent->SetSimulatePhysics(false);

	RootComponent = BoxComponent;

	//creating and attaching the sprite
	TileSprite = ObjectInitializer.CreateDefaultSubobject<UPaperSpriteComponent>(this, TEXT("PlayerFlipbook"));
	TileSprite->AttachTo(RootComponent);

	
}

// Called when the game starts or when spawned
void ABreakableTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableTile::OnHit() {

	this->Destroy();

}

