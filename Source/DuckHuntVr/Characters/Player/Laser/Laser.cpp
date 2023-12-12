#include "Laser.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "DuckHuntVr/Characters/Player/Hands/HandVisualizationInterface.h"
#include "DuckHuntVr/Characters/Player/HandsController/HandsController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ULaserComponent::ULaserComponent() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);

	struct FConstructorStatics {
		ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserNiagaraAsset;
		ConstructorHelpers::FObjectFinder<UPaperSprite> CrosshairSpriteAsset;
		FConstructorStatics()
		: LaserNiagaraAsset(TEXT("NiagaraSystem'/Game/DuckHuntVr/Characters/Player/Laser/VFX/NS_MenuLaser.NS_MenuLaser'")),
		  CrosshairSpriteAsset(TEXT("PaperSprite'/Game/DuckHuntVr/Characters/Player/Laser/Sprites/SPR_Crosshair.SPR_Crosshair'"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	NiagaraLaserAsset = ConstructorStatics.LaserNiagaraAsset.Object;
	CrosshairSpriteAsset = ConstructorStatics.CrosshairSpriteAsset.Object;
}

void ULaserComponent::Init(IHandVisualizationInterface* Parent) {
	SetNewHandVisualizationParent(Parent);
	UpdateType();
	RegisterComponent();
}

void ULaserComponent::UpdateType() {
	const auto NewType = ParentHandVisualization->GetHandsController()->GetLaserType();
	if (NewType == ELaserType::Laser) {
		if (CrosshairSprite) {
			CrosshairSprite->DestroyComponent();
			CrosshairSprite = nullptr;
		}
		check(!NiagaraLaser);

		NiagaraLaser = NewObject<UNiagaraComponent>(this);
		NiagaraLaser->SetAsset(NiagaraLaserAsset);
		NiagaraLaser->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		NiagaraLaser->RegisterComponent();
	}
	else if (NewType == ELaserType::Crosshair) {
		if (NiagaraLaser) {
			NiagaraLaser->DestroyComponent();
			NiagaraLaser = nullptr;
		}
		check(!CrosshairSprite);

		CrosshairSprite = NewObject<UPaperSpriteComponent>(this);
		CrosshairSprite->SetSprite(CrosshairSpriteAsset);
		CrosshairSprite->SetVisibility(false);
		CrosshairSprite->RegisterComponent();
	}
	else {
		check(NewType == ELaserType::None);
		if (NiagaraLaser) {
			NiagaraLaser->DestroyComponent();
			NiagaraLaser = nullptr;
		}
		else if (CrosshairSprite) {
			CrosshairSprite->DestroyComponent();
			CrosshairSprite = nullptr;
		}
		else {
			check(false);
		}
	}
}

void ULaserComponent::Destroy() {
	if (NiagaraLaser) {
		NiagaraLaser->DestroyComponent();
		NiagaraLaser = nullptr;
	}
	if (CrosshairSprite) {
		CrosshairSprite->DestroyComponent();
		CrosshairSprite = nullptr;
	}
	DestroyComponent();
}

void ULaserComponent::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);

	const auto Start = GetComponentLocation();
	const auto Fwd = GetForwardVector();
	auto End = Start + Fwd * 2000.0f;

    if (UKismetSystemLibrary::LineTraceSingle(
    	this,
    	Start, End,
    	TraceTypeQuery, false, {},
    	EDrawDebugTrace::None, HitResult, true,
    	FLinearColor::Black, FLinearColor::Red, 0
    )) {
		End = HitResult.ImpactPoint;
	}

	if (NiagaraLaser) {
		SetNiagaraLaserPosition(0, Start);
		SetNiagaraLaserPosition(1, End);
	}
	else if (CrosshairSprite) {
		if (HitResult.bBlockingHit) {
			CrosshairSprite->SetWorldLocationAndRotation(End, UKismetMathLibrary::MakeRotFromY(HitResult.ImpactNormal));
			if (!CrosshairSprite->IsVisible())
				CrosshairSprite->SetVisibility(true);
		}
		else if (CrosshairSprite->IsVisible())
			CrosshairSprite->SetVisibility(false);
	}
}

void ULaserComponent::SetNiagaraLaserPosition(const int Index, const FVector& Pos) const {
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVectorValue(
		NiagaraLaser,
		"User.PointArray", Index,
		Pos, false
	);
}
