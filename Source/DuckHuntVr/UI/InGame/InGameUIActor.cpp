#include "InGameUIActor.h"
#include "InGameWidget.h"
#include "Components/WidgetComponent.h"

AInGameUI::AInGameUI() {
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	InGameWidgetHolder = CreateDefaultSubobject<UWidgetComponent>(TEXT("InGameWidgetHolder"));
	InGameWidgetHolder->SetupAttachment(DefaultSceneRoot);
}

#if WITH_EDITOR
void AInGameUI::PostRegisterAllComponents() {
	Super::PostRegisterAllComponents();
	if (!GetWorld()->IsGameWorld()) {
		// so that it will update in editor
		if (const auto Widget = Cast<UInGameWidget>(InGameWidgetHolder->GetUserWidgetObject())) {
			InGameWidgetRef = Widget;
			InGameWidgetRef->HitPanel->SetTheme(DefaultTheme);
			SetSkyColor(SkyColor);
		}
	}
}
#endif

void AInGameUI::BeginPlay() {
	Super::BeginPlay();
	InGameWidgetRef = CastChecked<UInGameWidget>(InGameWidgetHolder->GetUserWidgetObject());
	InGameWidgetRef->HitPanel->SetTheme(DefaultTheme);
	SetSkyColor(SkyColor);
}

void AInGameUI::SetRound(const int32 NewRound) const {
	if (InGameWidgetRef.IsValid())
		InGameWidgetRef->RoundPanel->SetRound(NewRound);
}

void AInGameUI::SetSkyColor(const FLinearColor& NewColor) {
	SkyColor = NewColor;
	if (InGameWidgetRef.IsValid())
		InGameWidgetRef->ShotPanel->SetSkyColor(NewColor);
}

void AInGameUI::Redraw() const {
	InGameWidgetHolder->RequestRedraw();
}
