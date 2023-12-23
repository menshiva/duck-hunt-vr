#pragma once

#include "DuckHuntVr/Characters/Player/Laser/LaserBase.h"
#include "Hand/HandMotionControllerBase.h"
#include "HandsController.generated.h"

class AVrPawn;

DECLARE_DELEGATE(FHandsControllerGunFireDelegate);
DECLARE_DELEGATE(FHandsControllerMenuPressDelegate);
DECLARE_DELEGATE(FHandsControllerVisTypeChangeDelegate);

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandsController : public USceneComponent {
	GENERATED_BODY()
public:
	UHandsController();

	void Init(AVrPawn* VrPawn, EControllerHand DefaultPrimaryHand, ELaserType DefaultLaserType);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
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
	UPROPERTY(EditAnywhere, Category=Subcomponents)
	TSubclassOf<UHandMotionControllerBase> LeftMotionControllerClass;

	UPROPERTY(EditAnywhere, Category=Subcomponents)
	TSubclassOf<UHandMotionControllerBase> RightMotionControllerClass;
private:
	EVisualizationType GetNewVisualizationType() const;

	TWeakObjectPtr<AVrPawn> ParentVrPawn;

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
