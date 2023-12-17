#pragma once

#include "VrPawnBase.generated.h"

class UHandsControllerBase;
class UCameraComponent;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API AVrPawnBase : public APawn {
	GENERATED_BODY()
public:
	AVrPawnBase();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	void OnGunFire();
	void OnMenuPressed();

	FORCEINLINE const UHandsControllerBase* GetHandsController() const { return HandsController; }
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UHandsControllerBase> HandsControllerClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHandsControllerBase> HandsController;
};
