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
		HandType = Parent->GetHandType();
		check(HandType == EControllerHand::Left || HandType == EControllerHand::Right);
	}

	virtual void Destroy() = 0;

	virtual void SetPrimary(bool InitPrimary) {
		check(!IsPrimary()); // ensure that we call this function only after Init (both hands should be secondary). To swap hands, use SwapPrimary()
	}

	virtual void SwapPrimary(IHandVisualizationInterface* OtherHandVisualization) {
		check(this != OtherHandVisualization);
		check(IsPrimary()); // ensure that we call this function on primary hand
	}

	virtual bool IsPrimary() const = 0;

	FORCEINLINE EControllerHand GetHandType() const { return HandType; }
private:
	EControllerHand HandType = EControllerHand::Left;
};
