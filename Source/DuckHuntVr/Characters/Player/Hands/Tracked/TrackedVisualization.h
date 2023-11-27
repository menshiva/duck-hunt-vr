#pragma once

#include "OculusXRHandComponent.h"
#include "DuckHuntVr/Characters/Player/Hands/HandVisualizationInterface.h"
#include "TrackedVisualization.generated.h"

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UTrackedVisualizationBase : public UOculusXRHandComponent, public IHandVisualizationInterface {
	GENERATED_BODY()
public:
	UTrackedVisualizationBase();

	virtual void Init(UHandMotionController* Parent) override;
	virtual void Destroy() override;

	virtual void SetPrimary(bool InitPrimary) override;
	virtual void SwapPrimary(IHandVisualizationInterface* OtherHandVisualization) override;
	virtual bool IsPrimary() const override { return Primary; }

	// TODO
	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;
private:
	FORCEINLINE void UpdateHandMaterialColor() const {
		DynamicHandMaterial->SetScalarParameterValue(TEXT("Primary"), IsPrimary());
	}

	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicHandMaterial = nullptr;

	bool Primary = false;
};
