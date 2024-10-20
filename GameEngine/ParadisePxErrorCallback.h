#pragma once

#include <foundation/PxErrorCallback.h>
#include <PxPhysXConfig.h>

namespace physx
{
	class ParadisePxErrorCallback : public PxErrorCallback
	{
	public:
		ParadisePxErrorCallback();
		virtual	~ParadisePxErrorCallback();

		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) PX_OVERRIDE;
	};

}

