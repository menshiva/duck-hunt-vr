#include "HandMotionController.h"
#include "DuckHuntVr/Characters/Player/HandsController/HandsController.h"
#include "Visualization/Controller/HandController.h"

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

void UHandMotionController::Init(UHandsController* Parent, const EControllerHand Type) {
	ParentController = Parent;
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

void UHandMotionController::SetHandsVisualization() const {
	check(!HandMesh);
	// TODO: implement
}
