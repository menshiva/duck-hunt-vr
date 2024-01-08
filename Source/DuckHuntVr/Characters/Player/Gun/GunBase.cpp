#include "GunBase.h"
#include "Components/AudioComponent.h"
#include "DuckHuntVr/Characters/Player/Laser/LaserBase.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"

UGunBase::UGunBase() {
	PrimaryComponentTick.bCanEverTick = false;

	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));
	FireAudioComponent->SetUISound(true); // allows to play sound even when game is paused
}

void UGunBase::BeginPlay() {
	Super::BeginPlay();

	FireAudioComponent->SetSound(FireSound);
	FireAudioComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform); // fixes audio spatialization
	// FireAudioComponent->RegisterComponent(); // causes auto play

	if (LaserClass) {
		LaserComponent = NewObject<ULaserBase>(this, LaserClass);
		LaserComponent->SetupAttachment(this, TEXT("LaserSocket"));
		LaserComponent->RegisterComponent();
	}
}

void UGunBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (FireAudioComponent) {
		FireAudioComponent->DestroyComponent();
		FireAudioComponent = nullptr;
	}
	if (LaserComponent) {
		LaserComponent->DestroyComponent();
		LaserComponent = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void UGunBase::PlayFireEffects(const EControllerHand HandType) const {
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController()) {
		PlayerController->PlayHapticEffect(FireHapticFeedbackEffect, HandType);
		FireAudioComponent->Play();
	}
}

void UGunBase::ForceStopFireEffects() const {
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController())
		PlayerController->SetDisableHaptics(true);
	FireAudioComponent->Stop();
}
