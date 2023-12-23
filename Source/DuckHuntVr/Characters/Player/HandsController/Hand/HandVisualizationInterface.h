#pragma once

#include "HandVisualizationInterface.generated.h"

class UHandMotionControllerBase;
enum class ELaserType : uint8;
class UEnhancedInputLocalPlayerSubsystem;

UINTERFACE(MinimalAPI)
class UHandVisualizationInterface : public UInterface {
	GENERATED_BODY()
};

class DUCKHUNTVR_API IHandVisualizationInterface {
	GENERATED_BODY()
public:
	void Init(UHandMotionControllerBase* MotionController, bool Primary);
	void Destroy();

	virtual bool IsPrimary() const = 0;
	virtual void SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization);

	virtual void PlayFireEffects();
	virtual void UpdateLaserType(ELaserType NewType);
protected:
	EControllerHand GetHandType() const;

	FORCEINLINE void SetThisComponent(USceneComponent* ThisComponent) { UnderlyingComponent = ThisComponent; }

	virtual void InitImpl(USceneComponent* AttachmentParent, bool Primary) = 0;
	virtual void AddMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* Component) = 0;
	virtual void ClearMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem) const = 0;

	void Fire();
	void Menu();
private:
	TWeakObjectPtr<UHandMotionControllerBase> ParentMotionController;
	TWeakObjectPtr<USceneComponent> UnderlyingComponent;
};
