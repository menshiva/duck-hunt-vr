#pragma once

#include "OculusXRHandComponent.h"
#include "HandTracked.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandTracked : public UOculusXRHandComponent {
	GENERATED_BODY()
public:
	UHandTracked();
};
