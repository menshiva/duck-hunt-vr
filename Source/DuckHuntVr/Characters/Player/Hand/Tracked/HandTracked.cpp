#include "HandTracked.h"

UHandTracked::UHandTracked() {
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UPrimitiveComponent::SetCollisionProfileName(TEXT("NoCollision"), false);

	SetCastShadow(false);

	bUpdateHandScale = true;
}
