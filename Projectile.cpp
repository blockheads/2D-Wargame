// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Engine.h"
#include "BreakableTile.h"

// Sets default values
AProjectile::AProjectile(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Bullet = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, FName("Bullet"));
	Bullet->bHiddenInGame = false;
	Bullet->Mobility = EComponentMobility::Movable;
	RootComponent = Bullet;
	Bullet->SetNotifyRigidBodyCollision(true);

	Bullet->OnComponentHit.AddDynamic(this,&AProjectile::OnHit);
	ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &AProjectile::OnStop);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	BulletMesh->AttachTo(RootComponent);

	// Use this component to drive this projectile's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 1.0f;
	
	UWorld* World = GetWorld();
	if (World) {
		FTimerHandle DecayTimerHandle;
		World->GetTimerManager().SetTimer(DecayTimerHandle, this, &AProjectile::OnDecay, DecayTime, false);
	}
	

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::OnDecay() {
	//destroys actor once time runs out
	//this->Destroy();
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogTemp, Warning, TEXT("Hit event fired"));
	UE_LOG(LogTemp, Warning, TEXT("HitComponent: %s"), *OtherComponent->GetName());

	if (OtherActor->GetClass()->IsChildOf(ABreakableTile::StaticClass())) {
		UE_LOG(LogTemp, Warning, TEXT("Hit a tile"));

		//if it's a tile call the OnHit func of tile
		Cast<ABreakableTile>(OtherActor)->OnHit();
	}

}

void AProjectile::OnStop(UPrimitiveComponent* stopedComponent,) {

}