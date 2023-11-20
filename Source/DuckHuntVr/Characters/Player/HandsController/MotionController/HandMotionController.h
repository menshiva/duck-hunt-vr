#pragma once

#include "MotionControllerComponent.h"
#include "HandMotionController.generated.h"

class UHandController;
struct FHandControllerInitStatics;
class UHandTracked;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandMotionController : public UMotionControllerComponent {
	GENERATED_BODY()
public:
	UHandMotionController();

	void Init(EControllerHand Type);

	void ClearVisualization();

	UHandController* SetControllerVisualization(const FHandControllerInitStatics& InitData);

	UHandTracked* SetHandsVisualization();

	FORCEINLINE EControllerHand GetHandType() const { return Hand; }
	FORCEINLINE USkinnedMeshComponent* GetHandMesh() const { return HandMesh.Get(); }
private:
	EControllerHand Hand;

	UPROPERTY()
	TObjectPtr<USkinnedMeshComponent> HandMesh = nullptr;
};
