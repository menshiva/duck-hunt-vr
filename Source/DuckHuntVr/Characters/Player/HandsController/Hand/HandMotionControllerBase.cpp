#include "HandMotionControllerBase.h"
#include "Controller/ControllerVisualizationBase.h"
#include "Tracked/TrackedVisualizationBase.h"

UHandMotionControllerBase::UHandMotionControllerBase() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UHandMotionControllerBase::OnComponentDestroyed(const bool bDestroyingHierarchy) {
	if (VisualizationComponent) {
		VisualizationComponent->Destroy();
		VisualizationComponent = nullptr;
	}
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UHandMotionControllerBase::UpdateVisualization(const EVisualizationType NewVisualizationType, const bool Primary) {
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

	if (VisualizationComponent)
		VisualizationComponent->Init(this, HandType, Primary);
}
