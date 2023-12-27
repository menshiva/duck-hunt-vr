#include "DhLevelScriptActorInGame.h"
#include "DuckHuntVr/Characters/Player/VrPawn.h"
#include "DuckHuntVr/UI/InGame/InGameUIActor.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

ADhLevelScriptActorInGame::ADhLevelScriptActorInGame() {
	PrimaryActorTick.bCanEverTick = true;
}

#if WITH_EDITOR
void ADhLevelScriptActorInGame::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	if (!InGameUI.IsExplicitlyNull()) {
		InGameUI->SetSkyColor(DefaultSkyColor);
		InGameUI->Redraw();
	}
	SetSkyColor(DefaultSkyColor);
}
#endif

void ADhLevelScriptActorInGame::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	static double CurrentInGameUiRotationYaw = InGameUI->GetActorRotation().Yaw;

	const double NewCameraRotationYaw = Pawn->GetCameraRotationYaw();
	const double CameraRotationYawOffset = NewCameraRotationYaw - CurrentInGameUiRotationYaw;
	const double Angle = FMath::Abs(CameraRotationYawOffset);

	if (Angle > 60.0) {
		const double NewWidgetRotationYaw = CurrentInGameUiRotationYaw + (CameraRotationYawOffset > 0.0 ? 90.0 : -90.0);
		const auto DiffTransform = FTransform(FRotator(0.0, NewWidgetRotationYaw, 0.0) - FRotator(0.0, CurrentInGameUiRotationYaw, 0.0));
		InGameUI->SetActorTransform(InGameUI->GetActorTransform() * DiffTransform);
		CurrentInGameUiRotationYaw = NewWidgetRotationYaw;
	}
}

void ADhLevelScriptActorInGame::PlayPauseSound() const {
	UGameplayStatics::PlaySound2D(this, PauseSound);
}

void ADhLevelScriptActorInGame::OpenMainMenuLevel() {
	// we can just directly clear pause here instead of calling UDhGameStateBaseInGame::TogglePause(), since we are
	// going to open another level anyway
	const auto GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode->IsPaused())
		GameMode->ClearPause();
	OpenLevel(MainMenuLevel);
}
