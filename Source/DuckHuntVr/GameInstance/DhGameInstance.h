#pragma once

#include "DuckHuntVr/Characters/Player/HandsController/Hand/VisualizationType.h"
#include "DuckHuntVr/Characters/Player/Laser/LaserType.h"
#include "DhGameInstance.generated.h"

UCLASS(Config=Game, NotBlueprintable, NotBlueprintType)
class DUCKHUNTVR_API UDhGameInstance : public UGameInstance {
	GENERATED_BODY()
public:
	virtual void OnStart() override;

	void SavePrimaryHand(EControllerHand NewPrimaryHand);
	void SaveLaserType(ELaserType NewLaserType);
	void SetVisualizationTypeGameStartedWith(const EVisualizationType NewVisualizationType);

	FORCEINLINE EControllerHand GetPrimaryHand() const { return PrimaryHand; }
	FORCEINLINE ELaserType GetLaserType() const { return LaserType; }
	FORCEINLINE EVisualizationType GetVisualizationTypeGameStartedWith() const { return VisualizationTypeGameStartedWith; }
private:
	UPROPERTY(Config)
	EControllerHand PrimaryHand = EControllerHand::Right;

	UPROPERTY(Config)
	ELaserType LaserType = ELaserType::Laser;

	EVisualizationType VisualizationTypeGameStartedWith = EVisualizationType::None;
};
