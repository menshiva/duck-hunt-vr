#include "Gun.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "OculusXRInputFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "DuckHuntVr/Characters/Player/HandsController/HandsController.h"
#include "DuckHuntVr/Characters/Player/HandsController/MotionController/HandMotionController.h"
#include "DuckHuntVr/Characters/Player/HandsController/MotionController/Visualization/Controller/HandController.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"

UGun::UGun() {
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	UPrimitiveComponent::SetEnableGravity(false);
	bApplyImpulseOnDamage = false;
	bReplicatePhysicsToAutonomousProxy = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UStaticMeshComponent::SetCollisionProfileName(TEXT("NoCollision"), false);

	SetCastShadow(false);

	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));
	FireAudioComponent->SetupAttachment(this);

	FireAudioComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	FireAudioComponent->PrimaryComponentTick.bCanEverTick = false;
	FireAudioComponent->PrimaryComponentTick.bAllowTickOnDedicatedServer = false;
}

void UGun::Init(UHandController* Parent, const FTransform& Transform, const FGunInitStatics& InitData) {
	ParentHandController = Parent;

	SetRelativeTransform(Transform);
	SetStaticMesh(InitData.MeshAsset);
	FireAudioComponent->SetSound(InitData.SoundAsset);
	HapticFeedbackEffect = InitData.HapticFeedbackEffect;

	RegisterComponent();
	AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);

	InitFireMappingContext();
}

void UGun::Reattach(UHandController* NewParent, const FTransform& NewTransform) {
	RemoveFireMappingContext();
	DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

	ParentHandController = NewParent;

	SetRelativeTransform(NewTransform);

	AttachToComponent(NewParent, FAttachmentTransformRules::KeepRelativeTransform);

	InitFireMappingContext();
}

void UGun::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	FireAudioComponent->DestroyComponent();
	FireAudioComponent = nullptr;

	RemoveFireMappingContext();
}

void UGun::InitFireMappingContext() {
	if (const auto PlayerController = Cast<APlayerController>(ParentHandController->GetMotionController()->GetHandsController()->GetCharacter()->GetController())) {
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(ParentHandController->GetFireMappingContext(), 0);

		if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			EnhancedInputComponent->BindAction(ParentHandController->GetFireAction(), ETriggerEvent::Triggered, this, &UGun::Fire);
	}
}

void UGun::RemoveFireMappingContext() const {
	if (const auto PlayerController = Cast<APlayerController>(ParentHandController->GetMotionController()->GetHandsController()->GetCharacter()->GetController()))
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->RemoveMappingContext(ParentHandController->GetFireMappingContext());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UGun::Fire() {
	FireAudioComponent->Play();
	UOculusXRInputFunctionLibrary::PlayCurveHapticEffect(HapticFeedbackEffect.Get(), ParentHandController->GetMotionController()->GetHandType());
}
