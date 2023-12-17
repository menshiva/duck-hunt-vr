#pragma once

#include "HandVisualizationInterface.generated.h"

class AVrPawnBase;
class UEnhancedInputLocalPlayerSubsystem;
struct FInputActionValue;

UINTERFACE(MinimalAPI)
class UHandVisualizationInterface : public UInterface {
	GENERATED_BODY()
};

class DUCKHUNTVR_API IHandVisualizationInterface {
	GENERATED_BODY()
public:
	void Init(USceneComponent* AttachmentParent, EControllerHand bHandType, bool Primary);
	void Destroy();

	virtual bool IsPrimary() const = 0;
	virtual void SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization);

	virtual void PlayFireEffects();
	virtual void UpdateLaserType();
protected:
	FORCEINLINE EControllerHand GetHandType() const { return HandType; }

	FORCEINLINE void SetThisComponent(USceneComponent* ThisComponent) { UnderlyingComponent = ThisComponent; }

	virtual void InitImpl(USceneComponent* AttachmentParent, bool Primary) = 0;
	virtual void AddMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* Component) = 0;
	virtual void ClearMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem) const = 0;

	void Fire();
	void Menu();
private:
	const APlayerController* GetPlayerController() const;
	static AVrPawnBase* GetVrPawn(const APlayerController* PlayerController);

	TWeakObjectPtr<USceneComponent> UnderlyingComponent;
	EControllerHand HandType = EControllerHand::Left;
};
