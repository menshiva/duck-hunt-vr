#pragma once

#include "VrPawnBase.generated.h"

class UHandsController;
class UCameraComponent;
class UWidgetComponent;
class UInGameWidget;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API AVrPawnBase : public APawn {
	GENERATED_BODY()
public:
	AVrPawnBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void OnGunFire();
	void OnMenuPressed();

	FORCEINLINE const UHandsController* GetHandsController() const { return HandsController; }
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	// TODO: remove BlueprintReadOnly
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHandsController> HandsController;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> InGameWidgetHolder;
private:
	TWeakObjectPtr<UInGameWidget> InGameWidget;

	int32 BulletsNum = 3;
};
