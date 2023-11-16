#include "Gun.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"
#include "OculusXRInputFunctionLibrary.h"

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

	ShootSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootSound"));
	ShootSound->SetupAttachment(this);

	ShootSound->PrimaryComponentTick.bStartWithTickEnabled = false;
	ShootSound->PrimaryComponentTick.bCanEverTick = false;
	ShootSound->PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinder<UStaticMesh> GunMesh;
		ConstructorHelpers::FObjectFinder<USoundCue> GunSound;
		ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Curve> GunHaptic;

		FConstructorStatics() :
		GunMesh(TEXT("/Script/Engine.StaticMesh'/Game/DuckHuntVr/Characters/Player/Gun/Model/SM_Gun.SM_Gun'")),
		GunSound(TEXT("/Script/Engine.SoundCue'/Game/DuckHuntVr/Characters/Player/Gun/Audio/A_Gun_Cue.A_Gun_Cue'")),
		GunHaptic(TEXT("/Script/Engine.HapticFeedbackEffect_Curve'/Game/DuckHuntVr/Characters/Player/Gun/Haptics/HFC_GunFire.HFC_GunFire'"))
		{}
	};
	static const FConstructorStatics ConstructorStatics;

	UStaticMeshComponent::SetStaticMesh(ConstructorStatics.GunMesh.Object);
	ShootSound->SetSound(ConstructorStatics.GunSound.Object);
	HapticFeedbackEffect = ConstructorStatics.GunHaptic.Object;
}

void UGun::Shoot(const EControllerHand Hand) const {
	ShootSound->Play();
	UOculusXRInputFunctionLibrary::PlayCurveHapticEffect(HapticFeedbackEffect, Hand);
}
