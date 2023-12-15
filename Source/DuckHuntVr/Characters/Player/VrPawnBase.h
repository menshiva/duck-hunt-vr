#pragma once

#include "VrPawnBase.generated.h"

class UCameraComponent;
class UHandsControllerBase;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API AVrPawnBase : public APawn {
	GENERATED_BODY()
public:
	AVrPawnBase();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

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
