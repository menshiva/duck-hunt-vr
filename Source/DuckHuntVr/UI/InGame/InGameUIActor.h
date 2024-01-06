#pragma once

#include "Theme.h"
#include "InGameUIActor.generated.h"

class UWidgetComponent;
class UInGameWidget;

UCLASS(NotBlueprintable, NotBlueprintType, Placeable)
class DUCKHUNTVR_API AInGameUI : public AActor {
	GENERATED_BODY()
public:
	AInGameUI();

#if WITH_EDITOR
	virtual void PostRegisterAllComponents() override;
#endif
	virtual void BeginPlay() override;

	void SetStateInfoFlyAway() const;
	void SetStateInfoPause() const;
	void ClearStateInfo() const;

	void SetSkyColor(const FLinearColor& NewColor);
	bool RemoveBullet() const;
	void ResetBullets() const;
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> InGameWidgetHolder;

	UPROPERTY(EditAnywhere, Category=Init)
	ETheme DefaultTheme = ETheme::Duck;

	UPROPERTY(EditAnywhere, Category=Init)
	FLinearColor SkyColor;
private:
	TWeakObjectPtr<UInGameWidget> InGameWidgetRef;
};
