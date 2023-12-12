#pragma once

#include "DuckHuntVr/Characters/Player/HandsController/MotionController/HandMotionController.h"
#include "HandVisualizationInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHandVisualizationInterface : public UInterface {
	GENERATED_BODY()
};

class DUCKHUNTVR_API IHandVisualizationInterface {
	GENERATED_BODY()
public:
	virtual void Init(UHandMotionController* Parent) {
		ParentMotionController = Parent;
	}

	virtual void SetPrimary(bool InitPrimary) {
		// ensure that we call this function only after Init (both hands should be secondary). Use SwapPrimary() to swap hands
		check(!IsPrimary());
	}

	virtual void SwapPrimary(IHandVisualizationInterface* OtherHandVisualization) {
		check(this != OtherHandVisualization);
		check(IsPrimary()); // ensure that we call this function on primary hand
	}

	virtual void UpdateLaserType() {
		check(IsPrimary());
	}

	virtual void Destroy() = 0;

	virtual bool IsPrimary() const = 0;

	FORCEINLINE UHandsController* GetHandsController() const { return ParentMotionController->GetHandsController(); }
	FORCEINLINE EControllerHand GetHandType() const { return ParentMotionController->GetHandType(); }
private:
	TWeakObjectPtr<UHandMotionController> ParentMotionController = nullptr;
};
