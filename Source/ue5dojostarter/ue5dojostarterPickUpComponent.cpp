// Copyright Epic Games, Inc. All Rights Reserved.

#include "ue5dojostarterPickUpComponent.h"

Uue5dojostarterPickUpComponent::Uue5dojostarterPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void Uue5dojostarterPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &Uue5dojostarterPickUpComponent::OnSphereBeginOverlap);
}

void Uue5dojostarterPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	Aue5dojostarterCharacter* Character = Cast<Aue5dojostarterCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
