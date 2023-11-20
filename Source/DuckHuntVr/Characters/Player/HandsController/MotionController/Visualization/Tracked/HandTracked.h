#pragma once

#include "OculusXRHandComponent.h"
#include "HandTracked.generated.h"

class UHandMotionController;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandTracked : public UOculusXRHandComponent {
	GENERATED_BODY()
public:
	UHandTracked();

	void Init(UHandMotionController* Parent);

	void SetPrimary(bool IsPrimary, UMaterialInstance* AccordingMaterial);

	FORCEINLINE bool IsPrimary() const { return Primary; }
private:
	bool Primary = false;
};
