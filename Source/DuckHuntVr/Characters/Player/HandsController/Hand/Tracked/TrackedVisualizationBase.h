#pragma once

#include "OculusXRHandComponent.h"
#include "DuckHuntVr/Characters/Player/HandsController/Hand/HandVisualizationInterface.h"
#include "TrackedVisualizationBase.generated.h"

class ULaserBase;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UTrackedVisualizationBase : public UOculusXRHandComponent, public IHandVisualizationInterface {
	GENERATED_BODY()
public:
	UTrackedVisualizationBase();

	virtual void Destroy() override { DestroyComponent(); }
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	virtual bool IsPrimary() const override { return static_cast<bool>(LaserComponent); }
	virtual void SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) override;
	virtual void UpdateLaserType() override;

	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;
protected:
	virtual void InitImpl(USceneComponent* AttachmentParent, bool Primary) override;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<ULaserBase> LaserClass;
private:
	FORCEINLINE void UpdateHandMaterialColor() const {
		DynamicHandMaterial->SetScalarParameterValue(TEXT("Primary"), IsPrimary());
	}

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicHandMaterial;

	UPROPERTY()
	TObjectPtr<ULaserBase> LaserComponent;
};
