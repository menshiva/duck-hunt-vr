#include "HandVisualizationInterface.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DuckHuntVr/Characters/Player/VrPawnBase.h"

static UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem(const APlayerController* PlayerController) {
	return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
}

void IHandVisualizationInterface::Init(USceneComponent* AttachmentParent, const EControllerHand bHandType, const bool Primary) {
	HandType = bHandType;
	InitImpl(AttachmentParent, Primary);
	if (const auto PlayerController = GetPlayerController())
		if (const auto Subsystem = GetEnhancedInputSubsystem(PlayerController))
			if (const auto Component = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
				AddMappingContexts(Subsystem, Component);
}

void IHandVisualizationInterface::Destroy() {
	if (UnderlyingComponent.IsValid()) {
		if (const auto PlayerController = GetPlayerController())
			if (const auto Subsystem = GetEnhancedInputSubsystem(PlayerController))
				ClearMappingContexts(Subsystem);
		UnderlyingComponent->DestroyComponent();
	}
	UnderlyingComponent = nullptr;
}

void IHandVisualizationInterface::SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) {
	check(this != SecondaryHandVisualization);
	check(IsPrimary());
	check(!SecondaryHandVisualization->IsPrimary());
}

void IHandVisualizationInterface::PlayFireEffects() {
	check(IsPrimary());
}

void IHandVisualizationInterface::UpdateLaserType() {
	check(IsPrimary());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void IHandVisualizationInterface::Fire() {
	if (IsPrimary())
		if (const auto Pawn = GetVrPawn())
			Pawn->OnGunFire();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void IHandVisualizationInterface::Menu() {
	if (const auto Pawn = GetVrPawn())
		Pawn->OnMenuPressed();
}

const APlayerController* IHandVisualizationInterface::GetPlayerController() const {
	if (UnderlyingComponent.IsValid())
		return UnderlyingComponent->GetWorld()->GetFirstPlayerController();
	return nullptr;
}

AVrPawnBase* IHandVisualizationInterface::GetVrPawn() const {
	if (const auto PlayerController = GetPlayerController())
		return CastChecked<AVrPawnBase>(PlayerController->GetPawn());
	return nullptr;
}
