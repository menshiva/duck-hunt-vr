#pragma once

#include "MotionControllerComponent.h"
#include "VisualizationType.h"
#include "DuckHuntVr/Characters/Player/Laser/LaserType.h"
#include "HandMotionControllerBase.generated.h"

class UHandsController;
class IHandVisualizationInterface;
class UControllerVisualizationBase;
class UTrackedVisualizationBase;

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class DUCKHUNTVR_API UHandMotionControllerBase : public UMotionControllerComponent {
	GENERATED_BODY()
public:
	UHandMotionControllerBase();

	void Init(UHandsController* HandsController, EControllerHand Hand);
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void UpdateVisualization(EVisualizationType NewVisualizationType, bool Primary, ELaserType LaserType);

	FORCEINLINE UHandsController* GetParentHandsController() const { return ParentHandsController.Get(); }
	FORCEINLINE EControllerHand GetHandType() const { return HandType; }
	FORCEINLINE IHandVisualizationInterface* GetVisualizationComponent() const { return VisualizationComponent.GetInterface(); }
protected:
	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UControllerVisualizationBase> ControllerVisualizationClass;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UTrackedVisualizationBase> TrackedVisualizationClass;
private:
	TWeakObjectPtr<UHandsController> ParentHandsController;

	EControllerHand HandType = EControllerHand::Left;

	UPROPERTY()
	TScriptInterface<IHandVisualizationInterface> VisualizationComponent;
};
