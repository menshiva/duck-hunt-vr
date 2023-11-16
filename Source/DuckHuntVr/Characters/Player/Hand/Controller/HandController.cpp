#include "HandController.h"
#include "DuckHuntVr/Characters/Player/Gun/Gun.h"

UHandController::UHandController() {
	PrimaryComponentTick.bStartWithTickEnabled = false;
	// PrimaryComponentTick.bCanEverTick = false; // Tick is needed for HasGun and animations to work
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	SetAllowClothActors(false);
	bResetAfterTeleport = false;

	USkeletalMeshComponent::SetEnableGravity(false);
	bApplyImpulseOnDamage = false;
	bReplicatePhysicsToAutonomousProxy = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UPrimitiveComponent::SetCollisionProfileName(TEXT("NoCollision"), false);

	SetCastShadow(false);
}

bool UHandController::HasGun(UGun*& Gun) const {
	return (Gun = Cast<UGun>(GetChildComponent(0))) != nullptr;
}

void UHandController::ShootIfHasGun() const {
	UGun* Gun;
	if (HasGun(Gun))
		Gun->Shoot(Hand);
}
