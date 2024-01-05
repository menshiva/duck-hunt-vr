#pragma once

#include "DuckHuntVr/Characters/Player/Laser/LaserType.h"
#include "Hand/VisualizationType.h"
#include "HandsController.generated.h"

class AVrPawn;
class UHandMotionControllerBase;

DECLARE_DELEGATE_RetVal_OneParam(bool, FHandsControllerGunFireDelegate, void*);
DECLARE_DELEGATE(FHandsControllerMenuPressDelegate);
DECLARE_DELEGATE_OneParam(FHandsControllerVisTypeChangeDelegate, EVisualizationType);

UCLASS(NotBlueprintable, NotBlueprintType)
class DUCKHUNTVR_API UHandsController : public USceneComponent {
	GENERATED_BODY()
public:
	UHandsController();

	void Init(AVrPawn* VrPawn, EControllerHand DefaultPrimaryHand, EVisualizationType DefaultVisType, ELaserType DefaultLaserType);

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;

	void SetPrimaryHand(EControllerHand NewPrimaryHand);
	void SetLaserType(ELaserType NewLaserType);

	const APlayerController* GetPlayerController() const;
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
	static EVisualizationType GetNewVisualizationType();
	void UpdateVisualizationType(EVisualizationType NewType);

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
