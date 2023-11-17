#pragma once

#include "MotionControllerComponent.h"
#include "HandMotionController.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable, meta=(BlueprintSpawnableComponent))
class DUCKHUNTVR_API UHandMotionController : public UMotionControllerComponent {
	GENERATED_BODY()
public:
	UHandMotionController();
};
