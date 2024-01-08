#include "DhLevelScriptActorBase.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "DuckHuntVr/GameInstance/DhGameInstance.h"
#include "Kismet/GameplayStatics.h"

ADhLevelScriptActorBase::ADhLevelScriptActorBase() {
	PrimaryActorTick.bCanEverTick = true;
}

void ADhLevelScriptActorBase::BeginPlay() {
	GameInstance = CastChecked<UDhGameInstance>(GetGameInstance());
	Super::BeginPlay();
}

void ADhLevelScriptActorBase::OpenLevel(const TSoftObjectPtr<UWorld>& Level) {
	LevelNameToOpen = FPackageName::ObjectPathToPackageName(Level.ToString());

	FLoadPackageAsyncDelegate LoadPackageAsyncDelegate;
	LoadPackageAsyncDelegate.BindUObject(this, &ADhLevelScriptActorBase::OnAsyncMapLoadCompleted);
	LoadPackageAsync(LevelNameToOpen, LoadPackageAsyncDelegate, 0, PKG_ContainsMap);

	const auto Player = FadeInSequence->GetSequencePlayer();
	Player->OnPause.Clear();
	Player->OnPause.AddDynamic(this, &ADhLevelScriptActorBase::OnFadeInSequenceEnd);
	Player->Play();
}

void ADhLevelScriptActorBase::OnAsyncMapLoadCompleted(const FName&, UPackage*, const EAsyncLoadingResult::Type Result) {
	if (Result == EAsyncLoadingResult::Succeeded) {
		AsyncMapLoadCompleted = true;
		if (FadeInSequenceCompleted)
			OpenLevelImpl();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ADhLevelScriptActorBase::OnFadeInSequenceEnd() {
	FadeInSequenceCompleted = true;
	if (AsyncMapLoadCompleted)
		OpenLevelImpl();
}

void ADhLevelScriptActorBase::OpenLevelImpl() const {
	UGameplayStatics::OpenLevel(this, FName(LevelNameToOpen), true);
}
