#pragma once

#include "DuckHuntVr/Characters/Player/Laser/LaserBase.h"
#include "Hand/HandMotionControllerBase.h"
#include "HandsController.generated.h"

class AVrPawnBase;

DECLARE_DELEGATE(FHandsControllerGunFireDelegate);
DECLARE_DELEGATE(FHandsControllerMenuPressDelegate);
DECLARE_DELEGATE(FHandsControllerVisTypeChangeDelegate);

UCLASS(Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandsController : public USceneComponent {
	GENERATED_BODY()
public:
	UHandsController();

	void Init(AVrPawnBase* VrPawn, EControllerHand DefaultPrimaryHand, ELaserType DefaultLaserType);

	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;

	void SetPrimaryHand(EControllerHand NewPrimaryHand);
	void SetLaserType(ELaserType NewLaserType);
	void PlayFireEffects() const;

	APlayerController* GetPlayerController() const;
	FORCEINLINE EVisualizationType GetVisualizationType() const { return CurrentVisualizationType; }

	FHandsControllerGunFireDelegate OnGunFired;
	FHandsControllerMenuPressDelegate OnMenuPressed;
	FHandsControllerVisTypeChangeDelegate OnVisualizationTypeChanged;
protected:
	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UHandMotionControllerBase> LeftMotionControllerClass;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UHandMotionControllerBase> RightMotionControllerClass;
private:
	EVisualizationType GetNewVisualizationType() const;

	TWeakObjectPtr<AVrPawnBase> ParentVrPawn;

	EControllerHand PrimaryHand = EControllerHand::Right;
	EVisualizationType CurrentVisualizationType = EVisualizationType::None;
	ELaserType LaserType = ELaserType::Laser;

	UPROPERTY()
	TObjectPtr<UHandMotionControllerBase> LeftMotionController;

	UPROPERTY()
	TObjectPtr<UHandMotionControllerBase> RightMotionController;

	TWeakObjectPtr<UHandMotionControllerBase> PrimaryMotionController;
	TWeakObjectPtr<UHandMotionControllerBase> SecondaryMotionController;
};
