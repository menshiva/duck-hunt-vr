#pragma once

#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButtonWidget;
class ADhLevelScriptActorMenu;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UMainMenuWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButtonWidget> GameAButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButtonWidget> GameBButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButtonWidget> GameCButton;
private:
	UFUNCTION()
	void OnGameAButtonClicked();

	UFUNCTION()
	void OnGameBButtonClicked();

	UFUNCTION()
	void OnGameCButtonClicked();

	TWeakObjectPtr<ADhLevelScriptActorMenu> MenuLevelScript;
};
