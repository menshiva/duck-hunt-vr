#include "Gun.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

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

	m_pShootSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootSound"));
	m_pShootSound->SetupAttachment(this);

	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinder<UStaticMesh> GunMesh;
		ConstructorHelpers::FObjectFinder<USoundCue> GunSound;

		FConstructorStatics() :
		GunMesh(TEXT("/Script/Engine.StaticMesh'/Game/DuckHuntVr/Characters/Player/Gun/Model/SM_Gun.SM_Gun'")),
		GunSound(TEXT("/Script/Engine.SoundCue'/Game/DuckHuntVr/Characters/Player/Gun/Audio/A_Gun_Cue.A_Gun_Cue'"))
		{}
	};
	static const FConstructorStatics ConstructorStatics;

	UStaticMeshComponent::SetStaticMesh(ConstructorStatics.GunMesh.Object.Get());
	m_pShootSound->SetSound(ConstructorStatics.GunSound.Object.Get());
}

void UGun::Shoot() const {
	m_pShootSound->Play();
}
