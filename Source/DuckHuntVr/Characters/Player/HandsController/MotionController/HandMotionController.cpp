#include "HandMotionController.h"
#include "Visualization/Controller/HandController.h"
#include "Visualization/Tracked/HandTracked.h"

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
	if (HandMesh) {
		HandMesh->DestroyComponent(true);
		HandMesh = nullptr;
	}
}

UHandController* UHandMotionController::SetControllerVisualization(const FHandControllerInitStatics& InitData) {
	check(!HandMesh);
	const auto HandController = NewObject<UHandController>(this);
	HandController->Init(this, InitData);
	HandMesh = HandController;
	return HandController;
}

UHandTracked* UHandMotionController::SetHandsVisualization() {
	check(!HandMesh);
	const auto HandTracked = NewObject<UHandTracked>(this);
	HandTracked->Init(this);
	HandMesh = HandTracked;
	return HandTracked;
}
