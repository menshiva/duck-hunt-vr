#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ButtonWidget.generated.h"

class UImage;
class UTextWidget;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UButtonWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void SynchronizeProperties() override;

	void SetOnClickEvent(TFunction<void()> Event);
protected:
	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(DesignerRebuild))
	FText DefaultText;

	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(sRGB="true", DesignerRebuild))
	FLinearColor DefaultTextColor = FLinearColor::White;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> InternalButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> CursorImage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextWidget> Text;
private:
	UFUNCTION()
	void OnInternalButtonHovered();

	UFUNCTION()
	void OnInternalButtonUnhovered();

	UFUNCTION()
	void OnInternalButtonClicked();

	TFunction<void()> OnClick;
};
