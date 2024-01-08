#pragma once

#include "DhLevelScriptActorBase.h"
#include "DhLevelScriptActorMenu.generated.h"

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class DUCKHUNTVR_API ADhLevelScriptActorMenu : public ADhLevelScriptActorBase {
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;

	void OpenDuckLevel(int32 NumDucks);
	void OpenClayLevel();
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	TSoftObjectPtr<UWorld> DuckLevel;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TSoftObjectPtr<UWorld> ClayLevel;
private:
	bool IsHmdWorn = false;
};
