#include "GunBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/AudioComponent.h"
#include "DuckHuntVr/Characters/Player/HandsController/Hand/Controller/ControllerVisualizationBase.h"
#include "DuckHuntVr/Characters/Player/Laser/LaserBase.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"

UGunBase::UGunBase() {
	PrimaryComponentTick.bCanEverTick = false;

	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));
}

void UGunBase::OnComponentCreated() {
	Super::OnComponentCreated();

	FireAudioComponent->SetSound(FireSound);
	FireAudioComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform); // fixes audio spatialization
	// FireAudioComponent->RegisterComponent(); // causes auto play

	if (LaserClass) {
		LaserComponent = NewObject<ULaserBase>(this, LaserClass);
		LaserComponent->UpdateType();
		LaserComponent->SetupAttachment(this, TEXT("LaserSocket"));
		LaserComponent->RegisterComponent();
	}
}

void UGunBase::OnComponentDestroyed(const bool bDestroyingHierarchy) {
	FireAudioComponent->DestroyComponent();
	FireAudioComponent = nullptr;

	if (LaserComponent) {
		LaserComponent->DestroyComponent();
		LaserComponent = nullptr;
	}

	RemoveFireMappingContext(ParentControllerVisualizationComponent->GetGunFireMappingContext());
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UGunBase::Init(UControllerVisualizationBase* Parent) {
	if (!ParentControllerVisualizationComponent.IsExplicitlyNull())
		RemoveFireMappingContext(ParentControllerVisualizationComponent->GetGunFireMappingContext());

	ParentControllerVisualizationComponent = Parent;

	SetRelativeTransform(Parent->GetGunTransform());
	AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
	InitFireMappingContext(Parent->GetGunFireMappingContext(), Parent->GetGunFireAction());
}

void UGunBase::UpdateLaserType() const {
	LaserComponent->UpdateType();
}

void UGunBase::InitFireMappingContext(const UInputMappingContext* FireMappingContext, const UInputAction* FireAction) {
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController()) {
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(FireMappingContext, 0);

		if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UGunBase::Fire);
	}
}

void UGunBase::RemoveFireMappingContext(const UInputMappingContext* FireMappingContext) const {
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController())
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->RemoveMappingContext(FireMappingContext);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UGunBase::Fire() {
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController()) {
		PlayerController->PlayHapticEffect(FireHapticFeedbackEffect, ParentControllerVisualizationComponent->GetHandType());
		FireAudioComponent->Play();
	}
}
