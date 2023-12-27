#pragma once

#include "DhGameStateBase.h"
#include "DhGameStateBaseInGame.generated.h"

class ADhLevelScriptActorInGame;

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class DUCKHUNTVR_API ADhGameStateBaseInGame : public ADhGameStateBase {
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void TogglePause() override;
	virtual void OnVisualizationTypeChanged(EVisualizationType NewType) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	bool FlyAwayEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category=Init, meta=(EditCondition="FlyAwayEnabled"))
	FLinearColor FlyAwaySkyColor;
private:
	TWeakObjectPtr<ADhLevelScriptActorInGame> LevelScriptActor;

	bool AllowedToUnpause = true;
};
