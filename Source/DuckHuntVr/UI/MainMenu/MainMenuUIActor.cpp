#include "MainMenuUIActor.h"
#include "Components/WidgetComponent.h"

AMainMenuUI::AMainMenuUI() {
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	MainMenuWidgetHolder = CreateDefaultSubobject<UWidgetComponent>(TEXT("MainMenuWidgetHolder"));
	MainMenuWidgetHolder->SetupAttachment(DefaultSceneRoot);
}
