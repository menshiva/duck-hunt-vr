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

	FORCEINLINE void SetDucksNum(const int32 NewDucksNum) { DucksNum = NewDucksNum; }
	FORCEINLINE void SetGameMapOpenedFromMenu(const bool NewGameMapOpenedFromMenu) { GameMapOpenedFromMenu = NewGameMapOpenedFromMenu; }
	FORCEINLINE void SetGameVisualizationType(const EVisualizationType NewVisualizationType) { GameVisualizationType = NewVisualizationType; }

	FORCEINLINE EControllerHand GetPrimaryHand() const { return PrimaryHand; }
	FORCEINLINE ELaserType GetLaserType() const { return LaserType; }
	FORCEINLINE int32 GetDucksNum() const { return DucksNum; }
	FORCEINLINE bool HasGameMapOpenedFromMenu() const { return GameMapOpenedFromMenu; }
	FORCEINLINE EVisualizationType GetGameVisualizationType() const { return GameVisualizationType; }
private:
	UPROPERTY(Config)
	EControllerHand PrimaryHand = EControllerHand::Right;

	UPROPERTY(Config)
	ELaserType LaserType = ELaserType::Laser;

	int32 DucksNum = 0;
	bool GameMapOpenedFromMenu = false; // TODO: remove
	EVisualizationType GameVisualizationType = EVisualizationType::None;
};
