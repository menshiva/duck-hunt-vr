#include "ControllerVisualizationBase.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "DuckHuntVr/Characters/Player/Gun/GunBase.h"

UControllerVisualizationBase::UControllerVisualizationBase() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UControllerVisualizationBase::OnComponentDestroyed(const bool bDestroyingHierarchy) {
	if (GunComponent) {
		GunComponent->DestroyComponent();
		GunComponent = nullptr;
	}
	RemoveAnimMappingContext();
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UControllerVisualizationBase::SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) {
	IHandVisualizationInterface::SwapPrimary(SecondaryHandVisualization);
	const auto Secondary = CastChecked<UControllerVisualizationBase>(SecondaryHandVisualization);

	GunComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Swap(GunComponent, Secondary->GunComponent);
	Secondary->GunComponent->AttachToComponent(Secondary, FAttachmentTransformRules::KeepRelativeTransform);

	Secondary->GunComponent->Init(Secondary);
}

void UControllerVisualizationBase::UpdateLaserType() {
	IHandVisualizationInterface::UpdateLaserType();
	GunComponent->UpdateLaserType();
}

void UControllerVisualizationBase::InitImpl(USceneComponent* AttachmentParent, const bool Primary) {
	SetupAttachment(AttachmentParent);
	RegisterComponent();

	InitAnimMappingContext();

	if (Primary) {
		GunComponent = NewObject<UGunBase>(this, GunClass);
		GunComponent->Init(this);
		GunComponent->SetupAttachment(this);
		GunComponent->RegisterComponent();
	}
}

void UControllerVisualizationBase::InitAnimMappingContext() {
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController()) {
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(AnimMappingContext, 0);

		if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
			// don't know why, but binding with for example ETriggerEvent::Started | ETriggerEvent::Completed doesn't work

			EnhancedInputComponent->BindAction(AnimPointCapTouchAction, ETriggerEvent::Started, this, &UControllerVisualizationBase::AnimPointCapTouchActionEvent);
			EnhancedInputComponent->BindAction(AnimPointCapTouchAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimPointCapTouchActionEvent);

			EnhancedInputComponent->BindAction(AnimTriggerAction, ETriggerEvent::Triggered, this, &UControllerVisualizationBase::AnimTriggerActionEvent);
			EnhancedInputComponent->BindAction(AnimTriggerAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimTriggerActionEvent);

			EnhancedInputComponent->BindAction(AnimThumbCapTouchAction, ETriggerEvent::Started, this, &UControllerVisualizationBase::AnimThumbCapTouchActionEvent);
			EnhancedInputComponent->BindAction(AnimThumbCapTouchAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimThumbCapTouchActionEvent);

			EnhancedInputComponent->BindAction(AnimGripAction, ETriggerEvent::Triggered, this, &UControllerVisualizationBase::AnimGripActionEvent);
			EnhancedInputComponent->BindAction(AnimGripAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimGripActionEvent);
		}
	}
}

void UControllerVisualizationBase::RemoveAnimMappingContext() const {
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController())
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->RemoveMappingContext(AnimMappingContext);
}

void UControllerVisualizationBase::AnimPointCapTouchActionEvent(const FInputActionValue& Value) {
	IsPointing = Value.Get<bool>();
}

void UControllerVisualizationBase::AnimTriggerActionEvent(const FInputActionValue& Value) {
	TriggerAxis = Value.Get<float>();
}

void UControllerVisualizationBase::AnimThumbCapTouchActionEvent(const FInputActionValue& Value) {
	IsThumbUp = Value.Get<bool>();
}

void UControllerVisualizationBase::AnimGripActionEvent(const FInputActionValue& Value) {
	GripAxis = Value.Get<float>();
}
