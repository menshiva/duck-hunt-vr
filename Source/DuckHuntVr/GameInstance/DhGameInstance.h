#pragma once

#include "DuckHuntVr/Characters/Player/HandsController/Hand/VisualizationType.h"
#include "DuckHuntVr/Characters/Player/Laser/LaserType.h"
#include "DhGameInstance.generated.h"

UCLASS(Config=Game, NotBlueprintable, BlueprintType)
class DUCKHUNTVR_API UDhGameInstance : public UGameInstance {
	GENERATED_BODY()
public:
	virtual void OnStart() override;

	void SavePrimaryHand(EControllerHand NewPrimaryHand);
	void SaveLaserType(ELaserType NewLaserType);

	FORCEINLINE void SetGameMapOpenedFromMenu(const bool NewGameMapOpenedFromMenu) {
		GameMapOpenedFromMenu = NewGameMapOpenedFromMenu;
	}

	FORCEINLINE void SetGameVisualizationType(const EVisualizationType NewVisualizationType) {
		GameVisualizationType = NewVisualizationType;
	}

	FORCEINLINE EControllerHand GetPrimaryHand() const { return PrimaryHand; }
	FORCEINLINE ELaserType GetLaserType() const { return LaserType; }
	FORCEINLINE bool HasGameMapOpenedFromMenu() const { return GameMapOpenedFromMenu; }
	FORCEINLINE EVisualizationType GetGameVisualizationType() const { return GameVisualizationType; }
private:
	UPROPERTY(Config)
	EControllerHand PrimaryHand = EControllerHand::Right;

	UPROPERTY(Config)
	ELaserType LaserType = ELaserType::Laser;

	bool GameMapOpenedFromMenu = false; // TODO: remove
	EVisualizationType GameVisualizationType = EVisualizationType::None;
};
