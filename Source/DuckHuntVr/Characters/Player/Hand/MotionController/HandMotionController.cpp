#include "HandMotionController.h"
#include "DuckHuntVr/Characters/Player/Hand/Controller/HandController.h"
#include "DuckHuntVr/Characters/Player/Hand/Tracked/HandTracked.h"

UHandMotionController::UHandMotionController() {
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	UPrimitiveComponent::SetEnableGravity(false);
	bApplyImpulseOnDamage = false;
	bReplicatePhysicsToAutonomousProxy = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UPrimitiveComponent::SetCollisionProfileName(TEXT("NoCollision"), false);
}
