#pragma once

#include "MotionControllerComponent.h"
#include "HandMotionController.generated.h"

class UHandsController;
class UControllerVisualizationBase;
class UTrackedVisualizationBase;
class IHandVisualizationInterface;

enum class EVisualizationType : uint8_t {
	None, Controller, Tracked
};

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandMotionController : public UMotionControllerComponent {
	GENERATED_BODY()
public:
	UHandMotionController();

	void Init(UHandsController* Parent, EControllerHand Type);

	void ClearVisualization();
	void SetControllerVisualization(TSubclassOf<UControllerVisualizationBase> ControllerVisualizationClass);
	void SetTrackedVisualization(TSubclassOf<UTrackedVisualizationBase> TrackedVisualizationClass);

	FORCEINLINE UHandsController* GetHandsController() const { return ParentController.Get(); }
	FORCEINLINE EControllerHand GetHandType() const { return Hand; }
	FORCEINLINE IHandVisualizationInterface* GetVisualizationComponent() const { return VisualizationComponent.GetInterface(); }
private:
	TWeakObjectPtr<UHandsController> ParentController = nullptr;

	EControllerHand Hand;

	UPROPERTY()
	TScriptInterface<IHandVisualizationInterface> VisualizationComponent = nullptr;
};
