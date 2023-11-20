#include "HandController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "DuckHuntVr/Characters/Player/HandsController/HandsController.h"
#include "DuckHuntVr/Characters/Player/HandsController/MotionController/HandMotionController.h"
#include "Gun/Gun.h"

UHandController::UHandController() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	SetAllowClothActors(false);
	bResetAfterTeleport = false;

	USkeletalMeshComponent::SetEnableGravity(false);
	bApplyImpulseOnDamage = false;
	bReplicatePhysicsToAutonomousProxy = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UPrimitiveComponent::SetCollisionProfileName(TEXT("NoCollision"), false);

	SetCastShadow(false);
}

void UHandController::Init(UHandMotionController* Parent, const FHandControllerInitStatics& InitData) {
	ParentMotionController = Parent;
	AnimMappingContext = InitData.AnimMappingContext;
	FireMappingContext = InitData.FireMappingContext;
	FireAction = InitData.FireAction;

	SetRelativeTransform(InitData.Transform);
	SetAnimClass(InitData.AnimClass);
	SetSkeletalMesh(InitData.SkeletalMeshAsset);

	RegisterComponent();
	AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);

	InitAnimMappingContext(InitData);
}

void UHandController::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	if (Gun) {
		Gun->DestroyComponent();
		Gun = nullptr;
	}

	RemoveAnimMappingContext();
}

void UHandController::InitAnimMappingContext(const FHandControllerInitStatics& InitData) {
	if (const auto PlayerController = Cast<APlayerController>(ParentMotionController->GetHandsController()->GetCharacter()->GetController())) {
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(InitData.AnimMappingContext, 0);

		if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
			// don't know why, but binding with for example ETriggerEvent::Started | ETriggerEvent::Completed doesn't work

			EnhancedInputComponent->BindAction(InitData.AnimPointCapTouchAction, ETriggerEvent::Started, this, &UHandController::AnimPointCapTouchActionEvent);
			EnhancedInputComponent->BindAction(InitData.AnimPointCapTouchAction, ETriggerEvent::Completed, this, &UHandController::AnimPointCapTouchActionEvent);

			EnhancedInputComponent->BindAction(InitData.AnimTriggerAction, ETriggerEvent::Triggered, this, &UHandController::AnimTriggerActionEvent);
			EnhancedInputComponent->BindAction(InitData.AnimTriggerAction, ETriggerEvent::Completed, this, &UHandController::AnimTriggerActionEvent);

			EnhancedInputComponent->BindAction(InitData.AnimThumbCapTouchAction, ETriggerEvent::Started, this, &UHandController::AnimThumbCapTouchActionEvent);
			EnhancedInputComponent->BindAction(InitData.AnimThumbCapTouchAction, ETriggerEvent::Completed, this, &UHandController::AnimThumbCapTouchActionEvent);

			EnhancedInputComponent->BindAction(InitData.AnimGripAction, ETriggerEvent::Triggered, this, &UHandController::AnimGripActionEvent);
			EnhancedInputComponent->BindAction(InitData.AnimGripAction, ETriggerEvent::Completed, this, &UHandController::AnimGripActionEvent);
		}
	}
}

void UHandController::RemoveAnimMappingContext() const {
	if (const auto PlayerController = Cast<APlayerController>(ParentMotionController->GetHandsController()->GetCharacter()->GetController()))
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->RemoveMappingContext(AnimMappingContext.Get());
}

void UHandController::AnimPointCapTouchActionEvent(const FInputActionValue& Value) {
	IsPointing = Value.Get<bool>();
}

void UHandController::AnimTriggerActionEvent(const FInputActionValue& Value) {
	TriggerAxis = Value.Get<float>();
}

void UHandController::AnimThumbCapTouchActionEvent(const FInputActionValue& Value) {
	IsThumbUp = Value.Get<bool>();
}

void UHandController::AnimGripActionEvent(const FInputActionValue& Value) {
	GripAxis = Value.Get<float>();
}
