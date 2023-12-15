#pragma once

#include "HandVisualizationInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHandVisualizationInterface : public UInterface {
	GENERATED_BODY()
};

class DUCKHUNTVR_API IHandVisualizationInterface {
	GENERATED_BODY()
public:
	void Init(USceneComponent* AttachmentParent, const EControllerHand bHandType, const bool Primary) {
		HandType = bHandType;
		InitImpl(AttachmentParent, Primary);
	}

	virtual void Destroy() = 0;

	virtual bool IsPrimary() const = 0;

	virtual void SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) {
		check(this != SecondaryHandVisualization);
		check(IsPrimary());
		check(!SecondaryHandVisualization->IsPrimary());
	}

	virtual void UpdateLaserType() {
		check(IsPrimary());
	}

	FORCEINLINE EControllerHand GetHandType() const { return HandType; }
protected:
	virtual void InitImpl(USceneComponent* AttachmentParent, bool Primary) = 0;
private:
	EControllerHand HandType = EControllerHand::Left;
};
