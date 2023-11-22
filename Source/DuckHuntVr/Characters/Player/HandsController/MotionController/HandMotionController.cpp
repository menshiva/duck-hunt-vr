#include "HandMotionController.h"
#include "DuckHuntVr/Characters/Player/Hands/Controller/ControllerVisualization.h"
#include "DuckHuntVr/Characters/Player/Hands/Tracked/TrackedVisualization.h"

UHandMotionController::UHandMotionController() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	UPrimitiveComponent::SetEnableGravity(false);
	bApplyImpulseOnDamage = false;
	bReplicatePhysicsToAutonomousProxy = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UPrimitiveComponent::SetCollisionProfileName(TEXT("NoCollision"), false);
}

void UHandMotionController::Init(const EControllerHand Type) {
	Hand = Type;
	if (Hand == EControllerHand::Left)
		MotionSource = IMotionController::LeftHandSourceId;
	else {
		check(Hand == EControllerHand::Right);
		MotionSource = IMotionController::RightHandSourceId;
	}
}

void UHandMotionController::ClearVisualization() {
	if (VisualizationComponent) {
		VisualizationComponent->Destroy();
		VisualizationComponent = nullptr;
	}
}

void UHandMotionController::SetControllerVisualization(const TSubclassOf<UControllerVisualizationBase> ControllerVisualizationClass) {
	const auto ControllerVisualization = NewObject<UControllerVisualizationBase>(this, ControllerVisualizationClass);
	ControllerVisualization->Init(this);
	VisualizationComponent = ControllerVisualization;
}

void UHandMotionController::SetTrackedVisualization(const TSubclassOf<UTrackedVisualizationBase> TrackedVisualizationClass) {
	const auto TrackedVisualization = NewObject<UTrackedVisualizationBase>(this, TrackedVisualizationClass);
	TrackedVisualization->Init(this);
	VisualizationComponent = TrackedVisualization;
}
