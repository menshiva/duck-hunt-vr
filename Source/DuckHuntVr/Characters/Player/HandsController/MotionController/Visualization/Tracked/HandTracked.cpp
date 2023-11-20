#include "HandTracked.h"
#include "DuckHuntVr/Characters/Player/HandsController/MotionController/HandMotionController.h"

UHandTracked::UHandTracked() {
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UPrimitiveComponent::SetCollisionProfileName(TEXT("NoCollision"), false);

	SetCastShadow(false);

	bUpdateHandScale = true;
}

void UHandTracked::Init(UHandMotionController* Parent) {
	const auto HandType = Parent->GetHandType();
	check(HandType == EControllerHand::Left || HandType == EControllerHand::Right);
	const EOculusXRHandType OculusHandType = HandType == EControllerHand::Left ? EOculusXRHandType::HandLeft : EOculusXRHandType::HandRight;

	SkeletonType = OculusHandType;
	MeshType = OculusHandType;

	RegisterComponent();
	AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
}

void UHandTracked::SetPrimary(const bool IsPrimary, UMaterialInstance* AccordingMaterial) {
	Primary = IsPrimary;
	MaterialOverride = AccordingMaterial;
	bSkeletalMeshInitialized = false; // Force the mesh to be reinitialized
}
