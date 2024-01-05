#include "LaserBase.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "PaperSpriteComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"

ULaserBase::ULaserBase() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;

	UActorComponent::SetAutoActivate(true);

	TargetTraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);

	WidgetInteractor = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractor"));
	WidgetInteractor->PointerIndex = 1;
	WidgetInteractor->InteractionDistance = MaxLaserDistance;
}

void ULaserBase::Activate(const bool bReset) {
	Super::Activate(bReset);

	WidgetInteractor->Activate();

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

	UiHit = false;
	WidgetInteractor->Deactivate();

	if (NiagaraLaser) {
		NiagaraLaser->Deactivate();
		NiagaraLaser->SetVisibility(false);
	}
	else if (CrosshairSprite)
		CrosshairSprite->SetVisibility(false);
}

void ULaserBase::BeginPlay() {
	Super::BeginPlay();
	WidgetInteractor->SetupAttachment(this);
	WidgetInteractor->RegisterComponent();
}

void ULaserBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (WidgetInteractor) {
		WidgetInteractor->DestroyComponent();
		WidgetInteractor = nullptr;
	}

	if (NiagaraLaser) {
		NiagaraLaser->DestroyComponent();
		NiagaraLaser = nullptr;
	}
	else if (CrosshairSprite) {
		CrosshairSprite->DestroyComponent();
		CrosshairSprite = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void ULaserBase::UpdateType(const ELaserType NewType) {
	if (Type == NewType)
		return;
	Type = NewType;

	if (Type == ELaserType::Laser) {
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
	else {
		check(Type == ELaserType::Crosshair || Type == ELaserType::None);
		if (NiagaraLaser) {
			NiagaraLaser->DestroyComponent();
			NiagaraLaser = nullptr;
		}
		if (!CrosshairSprite) {
			CrosshairSprite = NewObject<UPaperSpriteComponent>(this);
			CrosshairSprite->SetSprite(CrosshairSpriteAsset);
			CrosshairSprite->SetVisibility(false);
			CrosshairSprite->RegisterComponent();
		}
	}
}

void ULaserBase::ClickUI() const {
	WidgetInteractor->PressPointerKey(EKeys::LeftMouseButton);
	WidgetInteractor->ReleasePointerKey(EKeys::LeftMouseButton);
}

void ULaserBase::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);

	const auto Start = GetComponentLocation();
	const auto Fwd = GetForwardVector();
	auto End = Start + Fwd * MaxLaserDistance;

	UiHit = WidgetInteractor->IsOverInteractableWidget();
	if (UiHit) {
		HitResult = WidgetInteractor->GetLastHitResult();
		End = HitResult.ImpactPoint;
	}
	else if (UKismetSystemLibrary::LineTraceSingle(
    	this,
    	Start, End,
    	TargetTraceTypeQuery, false, {},
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
		if (GetWorld()->GetAuthGameMode()->IsPaused()) {
			// I couldn't find another way :(, NiagaraLaser->SetTickableWhenPaused(true) doesn't work
			NiagaraLaser->AdvanceSimulation(1, Dt);
		}
	}
	else if (CrosshairSprite) {
		if ((Type == ELaserType::Crosshair && HitResult.bBlockingHit) || UiHit) {
			CrosshairSprite->SetWorldLocationAndRotation(End, FRotationMatrix::MakeFromY(HitResult.ImpactNormal).Rotator());
			if (!CrosshairSprite->IsVisible())
				CrosshairSprite->SetVisibility(true);
		}
		else if (CrosshairSprite->IsVisible())
			CrosshairSprite->SetVisibility(false);
	}
}
