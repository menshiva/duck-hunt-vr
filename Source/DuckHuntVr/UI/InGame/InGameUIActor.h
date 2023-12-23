#pragma once

#include "InGameUIActor.generated.h"

class UWidgetComponent;

UCLASS(NotBlueprintable, NotBlueprintType, Placeable)
class DUCKHUNTVR_API AInGameUI : public AActor {
	GENERATED_BODY()
public:
	AInGameUI();
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> InGameWidgetHolder;
};
