#include "InGameUIActor.h"
#include "Components/WidgetComponent.h"

AInGameUI::AInGameUI() {
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	InGameWidgetHolder = CreateDefaultSubobject<UWidgetComponent>(TEXT("InGameWidgetHolder"));
	InGameWidgetHolder->SetupAttachment(DefaultSceneRoot);
}

void AInGameUI::PostRegisterAllComponents() {
	Super::PostRegisterAllComponents();
#if WITH_EDITOR
	// so that it will update in editor
	if (!GetWorld()->IsGameWorld()) {
		const auto InGameWidget = CastChecked<UInGameWidget>(InGameWidgetHolder->GetUserWidgetObject());
		InGameWidget->SetTheme(Theme);
	}
#endif
}

void AInGameUI::BeginPlay() {
	Super::BeginPlay();
	InGameWidgetRef = CastChecked<UInGameWidget>(InGameWidgetHolder->GetUserWidgetObject());
	InGameWidgetRef->SetTheme(Theme);
}
