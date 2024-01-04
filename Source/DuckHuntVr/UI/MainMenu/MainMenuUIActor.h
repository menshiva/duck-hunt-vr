#pragma once

#include "MainMenuUIActor.generated.h"

class UWidgetComponent;

UCLASS(NotBlueprintable, NotBlueprintType, Placeable)
class DUCKHUNTVR_API AMainMenuUI : public AActor {
	GENERATED_BODY()
public:
	AMainMenuUI();
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> MainMenuWidgetHolder;
};
