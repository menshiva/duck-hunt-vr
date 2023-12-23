#include "DhLevelScriptActorBase.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

void ADhLevelScriptActorBase::OpenLevel(const TSoftObjectPtr<UWorld>& Level) {
	LevelToOpen = &Level;

	const auto Player = FadeInSequence->GetSequencePlayer();
	Player->OnPause.Clear();
	Player->OnPause.AddDynamic(this, &ADhLevelScriptActorBase::OnFadeInSequenceEnd);

	Player->Play();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ADhLevelScriptActorBase::OnFadeInSequenceEnd() {
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, *LevelToOpen, true);
}
