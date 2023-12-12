#include "Gun.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/AudioComponent.h"
#include "DuckHuntVr/Characters/Player/Hands/Controller/ControllerVisualization.h"
#include "DuckHuntVr/Characters/Player/Laser/Laser.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"
#include "Kismet/GameplayStatics.h"

UGunComponent::UGunComponent() {
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

	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinder<UStaticMesh> GunMesh;
		ConstructorHelpers::FObjectFinder<USoundBase> FireSound;
		ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Curve> FireHapticEffect;
		FConstructorStatics()
		: GunMesh(TEXT("StaticMesh'/Game/DuckHuntVr/Characters/Player/Gun/Model/SM_Gun.SM_Gun'")),
		  FireSound(TEXT("SoundCue'/Game/DuckHuntVr/Characters/Player/Gun/Audio/A_Fire_Cue.A_Fire_Cue'")),
		  FireHapticEffect(TEXT("HapticFeedbackEffect_Curve'/Game/DuckHuntVr/Characters/Player/Gun/Haptics/HFC_Fire.HFC_Fire'"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	UStaticMeshComponent::SetStaticMesh(ConstructorStatics.GunMesh.Object);

	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));
	FireAudioComponent->SetSound(ConstructorStatics.FireSound.Object);
	FireAudioComponent->SetupAttachment(this);

	FireHapticFeedbackEffect = ConstructorStatics.FireHapticEffect.Object;

	LaserComponent = CreateDefaultSubobject<ULaserComponent>(TEXT("LaserComponent"));
	LaserComponent->SetupAttachment(this, TEXT("LaserSocket"));
}

void UGunComponent::Init(UControllerVisualizationBase* Parent) {
	SetNewParentControllerVisualization(Parent);
	FireAudioComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform); // fixes audio spatialization
	LaserComponent->Init(Parent);
	RegisterComponent();
}

void UGunComponent::SetNewParentControllerVisualization(UControllerVisualizationBase* NewParent) {
	if (!ParentControllerVisualizationComponent.IsExplicitlyNull()) {
		RemoveFireMappingContext(ParentControllerVisualizationComponent->GetGunInitData());
		DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	}

	ParentControllerVisualizationComponent = NewParent;
	const auto& InitData = NewParent->GetGunInitData();

	SetRelativeTransform(InitData.Transform);
	AttachToComponent(NewParent, FAttachmentTransformRules::KeepRelativeTransform);
	InitFireMappingContext(InitData);
}

void UGunComponent::UpdateLaserType() const {
	LaserComponent->UpdateType();
}

void UGunComponent::Destroy() {
	FireAudioComponent->DestroyComponent();
	FireAudioComponent = nullptr;

	LaserComponent->Destroy();
	LaserComponent = nullptr;

	RemoveFireMappingContext(ParentControllerVisualizationComponent->GetGunInitData());
	DestroyComponent();
}

void UGunComponent::InitFireMappingContext(const FGunInitData& HandInitData) {
	if (const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0)) {
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(HandInitData.FireMappingContext, 0);

		if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			EnhancedInputComponent->BindAction(HandInitData.FireAction, ETriggerEvent::Triggered, this, &UGunComponent::Fire);
	}
}

void UGunComponent::RemoveFireMappingContext(const FGunInitData& HandInitData) const {
	if (const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->RemoveMappingContext(HandInitData.FireMappingContext);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UGunComponent::Fire() {
	FireAudioComponent->Play();
	UGameplayStatics::GetPlayerController(this, 0)->PlayHapticEffect(
		FireHapticFeedbackEffect, ParentControllerVisualizationComponent->GetHandType()
	);
}
