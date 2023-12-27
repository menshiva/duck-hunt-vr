#pragma once

#include "DuckHuntVr/Characters/Player/HandsController/Hand/VisualizationType.h"
#include "GameFramework/GameStateBase.h"
#include "DhGameStateBase.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType)
class DUCKHUNTVR_API ADhGameStateBase : public AGameStateBase {
	GENERATED_BODY()
public:
	virtual void TogglePause() {}
	virtual void OnVisualizationTypeChanged(EVisualizationType NewType) {}
};
