#pragma once

#include "Laser/LaserBase.h"
#include "VrPawn.generated.h"

class UCameraComponent;
class UHandsController;

UCLASS(NotBlueprintable, NotBlueprintType, Placeable)
class DUCKHUNTVR_API AVrPawn : public APawn {
	GENERATED_BODY()
public:
	AVrPawn();

	virtual void BeginPlay() override;

	void SetPrimaryHand(EControllerHand NewPrimaryHand) const;
	void SetLaserType(ELaserType NewLaserType) const;
	static void ResetOrientationAndPosition();
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHandsController> HandsController;
private:
	void OnGunFired() const;
	void OnMenuPressed() const;
	void OnVisualizationTypeChanged() const;
};
