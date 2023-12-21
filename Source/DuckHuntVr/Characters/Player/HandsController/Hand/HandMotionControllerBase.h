#pragma once

#include "MotionControllerComponent.h"
#include "HandMotionControllerBase.generated.h"

class IHandVisualizationInterface;
class UControllerVisualizationBase;
class UTrackedVisualizationBase;

enum class EVisualizationType : uint8_t {
	None, Controller, Tracked
};

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandMotionControllerBase : public UMotionControllerComponent {
	GENERATED_BODY()
public:
	UHandMotionControllerBase();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	FORCEINLINE void SetHandType(const EControllerHand NewHandType) { HandType = NewHandType; }

	void UpdateVisualization(EVisualizationType NewVisualizationType, bool Primary);

	FORCEINLINE IHandVisualizationInterface* GetVisualizationComponent() const { return VisualizationComponent.GetInterface(); }
protected:
	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UControllerVisualizationBase> ControllerVisualizationClass;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UTrackedVisualizationBase> TrackedVisualizationClass;
private:
	EControllerHand HandType = EControllerHand::Left;

	UPROPERTY()
	TScriptInterface<IHandVisualizationInterface> VisualizationComponent;
};
