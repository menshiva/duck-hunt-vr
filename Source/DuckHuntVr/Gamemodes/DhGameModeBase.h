#pragma once

#include "GameFramework/GameModeBase.h"
#include "DhGameModeBase.generated.h"

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API ADhGameModeBase : public AGameModeBase {
	GENERATED_BODY()
public:
	ADhGameModeBase();

	virtual bool AllowPausing(APlayerController* PC = nullptr) override;
};
