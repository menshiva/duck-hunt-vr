#pragma once

#include "Engine/LevelScriptActor.h"
#include "DhLevelScriptActorBase.generated.h"

class ALevelSequenceActor;
class AVrPawn;

UCLASS(Abstract, NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API ADhLevelScriptActorBase : public ALevelScriptActor {
	GENERATED_BODY()
public:
	ADhLevelScriptActorBase();
protected:
	void OpenLevel(const TSoftObjectPtr<UWorld>& Level);

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<ALevelSequenceActor> FadeInSequence;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<AVrPawn> Pawn;
private:
	UFUNCTION()
	void OnFadeInSequenceEnd();

	const TSoftObjectPtr<UWorld>* LevelToOpen = nullptr;
};
