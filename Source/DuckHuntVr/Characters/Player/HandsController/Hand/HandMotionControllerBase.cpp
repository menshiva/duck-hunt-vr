#include "HandMotionControllerBase.h"
#include "Controller/ControllerVisualizationBase.h"
#include "DuckHuntVr/Characters/Player/HandsController/HandsController.h"
#include "Tracked/TrackedVisualizationBase.h"

UHandMotionControllerBase::UHandMotionControllerBase() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}

void UHandMotionControllerBase::Init(UHandsController* HandsController, const EControllerHand Hand) {
	ParentHandsController = HandsController;
	HandType = Hand;
}

void UHandMotionControllerBase::OnComponentDestroyed(const bool bDestroyingHierarchy) {
	if (VisualizationComponent) {
		VisualizationComponent->Destroy();
		VisualizationComponent = nullptr;
	}
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UHandMotionControllerBase::UpdateVisualization(const EVisualizationType NewVisualizationType, const bool Primary, const ELaserType LaserType) {
	if (VisualizationComponent) {
		VisualizationComponent->Destroy();
		VisualizationComponent = nullptr;
	}

	switch (NewVisualizationType) {
		case EVisualizationType::Controller:
			VisualizationComponent = NewObject<UControllerVisualizationBase>(this, ControllerVisualizationClass);
			break;
		case EVisualizationType::Tracked:
			VisualizationComponent = NewObject<UTrackedVisualizationBase>(this, TrackedVisualizationClass);
			break;
		case EVisualizationType::None:
			break;
		default:
			check(false);
	}

	if (VisualizationComponent) {
		VisualizationComponent->Init(this, Primary);
		if (Primary)
			VisualizationComponent->UpdateLaserType(LaserType);
	}
}
