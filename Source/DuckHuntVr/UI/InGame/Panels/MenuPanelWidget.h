#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuPanelWidget.generated.h"

class UButtonWidget;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UMenuPanelWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButtonWidget> RestartButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButtonWidget> OrientationButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButtonWidget> MainMenuButton;
};
