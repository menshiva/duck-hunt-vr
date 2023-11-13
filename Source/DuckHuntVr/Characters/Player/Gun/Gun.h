#pragma once

#include "Gun.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class DUCKHUNTVR_API UGun : public UStaticMeshComponent {
	GENERATED_BODY()
public:
	UGun();

	UFUNCTION(BlueprintCallable)
	void Shoot() const;
private:
	UPROPERTY()
	TObjectPtr<UAudioComponent> m_pShootSound;
};
