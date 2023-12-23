#pragma once

#include "InGameWidget.h"
#include "InGameUIActor.generated.h"

class UWidgetComponent;

UCLASS(NotBlueprintable, NotBlueprintType, Placeable)
class DUCKHUNTVR_API AInGameUI : public AActor {
	GENERATED_BODY()
public:
	AInGameUI();

	virtual void PostRegisterAllComponents() override;
	virtual void BeginPlay() override;

	FORCEINLINE UInGameWidget* GetWidget() const { return InGameWidgetRef.Get(); }
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> InGameWidgetHolder;

	UPROPERTY(EditAnywhere, Category=Init)
	ETheme Theme = ETheme::Duck;
private:
	TWeakObjectPtr<UInGameWidget> InGameWidgetRef;
};
