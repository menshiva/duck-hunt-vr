#pragma once

#include "LaserType.generated.h"

UENUM(BlueprintType)
enum class ELaserType : uint8 {
	None,
	Crosshair,
	Laser
};
