#pragma once

#include "MotionControllerComponent.h"
#include "HandMotionController.generated.h"

class UHandsController;
class UHandController;
struct FHandControllerInitStatics;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandMotionController : public UMotionControllerComponent {
	GENERATED_BODY()
public:
	UHandMotionController();

	void Init(UHandsController* Parent, EControllerHand Type);

	void ClearVisualization();
	UHandController* SetControllerVisualization(const FHandControllerInitStatics& InitData);
	void SetHandsVisualization() const;

	FORCEINLINE UHandsController* GetHandsController() const { return ParentController.Get(); }
	FORCEINLINE EControllerHand GetHandType() const { return Hand; }
private:
	TWeakObjectPtr<UHandsController> ParentController = nullptr;

	EControllerHand Hand;

	UPROPERTY()
	TObjectPtr<USkinnedMeshComponent> HandMesh = nullptr;
};
