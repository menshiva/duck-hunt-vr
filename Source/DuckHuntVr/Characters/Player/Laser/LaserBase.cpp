#include "LaserBase.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "PaperSpriteComponent.h"
#include "DuckHuntVr/Characters/Player/VrPawnBase.h"
#include "DuckHuntVr/Characters/Player/HandsController/HandsControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ULaserBase::ULaserBase() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;

	UActorComponent::SetAutoActivate(true);

	TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
}

void ULaserBase::Activate(const bool bReset) {
	Super::Activate(bReset);
	if (NiagaraLaser) {
		NiagaraLaser->Activate();
		NiagaraLaser->SetVisibility(true);
	}
	else if (CrosshairSprite) {
		// No need, will happen in TickComponent if needed
	}
}

void ULaserBase::Deactivate() {
	Super::Deactivate();
	if (NiagaraLaser) {
		NiagaraLaser->Deactivate();
		NiagaraLaser->SetVisibility(false);
	}
	else if (CrosshairSprite)
		CrosshairSprite->SetVisibility(false);
}

void ULaserBase::OnComponentDestroyed(const bool bDestroyingHierarchy) {
	if (NiagaraLaser) {
		NiagaraLaser->DestroyComponent();
		NiagaraLaser = nullptr;
	}
	else if (CrosshairSprite) {
		CrosshairSprite->DestroyComponent();
		CrosshairSprite = nullptr;
	}
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void ULaserBase::UpdateType() {
	if (const auto PlayerController = GetWorld()->GetFirstPlayerController()) {
		const auto VrPawn = CastChecked<AVrPawnBase>(PlayerController->GetPawn());
		const auto NewType = VrPawn->GetHandsController()->GetLaserType();

		if (NewType == ELaserType::Laser) {
			if (CrosshairSprite) {
				CrosshairSprite->DestroyComponent();
				CrosshairSprite = nullptr;
			}
			check(!NiagaraLaser);

			NiagaraLaser = NewObject<UNiagaraComponent>(this);
			NiagaraLaser->SetAsset(NiagaraLaserAsset);
			NiagaraLaser->SetupAttachment(this);
			NiagaraLaser->RegisterComponent();
			if (!IsActive()) {
				NiagaraLaser->Deactivate();
				NiagaraLaser->SetVisibility(false);
			}
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
		}
	}
}

void ULaserBase::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);

	const auto Start = GetComponentLocation();
	const auto Fwd = GetForwardVector();
	auto End = Start + Fwd * MaxLaserDistance;

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
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVectorValue(
			NiagaraLaser, "User.PointArray", 0,
			Start, false
		);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVectorValue(
			NiagaraLaser, "User.PointArray", 1,
			End, false
		);
		if (UGameplayStatics::IsGamePaused(this)) {
			// I couldn't find another way :(, NiagaraLaser->SetTickableWhenPaused(true) doesn't work
			NiagaraLaser->AdvanceSimulation(1, Dt);
		}
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
