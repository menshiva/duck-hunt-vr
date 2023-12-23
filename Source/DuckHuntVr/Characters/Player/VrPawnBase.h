#pragma once

#include "VrPawnBase.generated.h"

enum class ELaserType : uint8;
class UCameraComponent;
class UHandsController;
class UWidgetComponent;
class UInGameWidget;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API AVrPawnBase : public APawn {
	GENERATED_BODY()
public:
	AVrPawnBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void SetPrimaryHand(EControllerHand NewPrimaryHand) const;

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void SetLaserType(ELaserType NewLaserType) const;

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	static void ResetOrientationAndPosition();
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHandsController> HandsController;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> InGameWidgetHolder;
private:
	void OnGunFired() const;
	void OnMenuPressed() const;
	void OnVisualizationTypeChanged() const;

	TWeakObjectPtr<UInGameWidget> InGameWidget;
};
