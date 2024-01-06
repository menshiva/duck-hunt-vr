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
	InGameWidgetHolder->PrimaryComponentTick.bTickEvenWhenPaused = true;
	InGameWidgetRef = CastChecked<UInGameWidget>(InGameWidgetHolder->GetUserWidgetObject());
	InGameWidgetRef->HitPanel->SetTheme(DefaultTheme);
	SetSkyColor(SkyColor);
}

void AInGameUI::SetStateInfoFlyAway() const {
	InGameWidgetRef->SetStateInfo(EStateInfo::FlyAway);
}

void AInGameUI::SetStateInfoPause() const {
	InGameWidgetRef->SetStateInfo(EStateInfo::Pause);
}

void AInGameUI::ClearStateInfo() const {
	InGameWidgetRef->SetStateInfo(EStateInfo::None);
}

void AInGameUI::SetSkyColor(const FLinearColor& NewColor) {
	SkyColor = NewColor;
	if (InGameWidgetRef.IsValid())
		InGameWidgetRef->ShotPanel->SetSkyColor(NewColor);
}

bool AInGameUI::RemoveBullet() const {
	return InGameWidgetRef->ShotPanel->RemoveBullet();
}

void AInGameUI::ResetBullets() const {
	InGameWidgetRef->ShotPanel->ResetBullets();
}
