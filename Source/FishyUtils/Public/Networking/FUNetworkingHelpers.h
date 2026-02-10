// By hzFishy - 2026 - Do whatever you want with it.
#pragma once

#include "Net/Core/PushModel/PushModel.h"


/** For push model */
#define FU_NET_MARKDIRTY(PropertyName) MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PropertyName, this)


/** Call dirty for push model and call OnRep func (usually for listen servers) */
#define FU_NET_MARKDIRTY_ONREPCALL(PropertyName) \
	FU_NET_MARKDIRTY(PropertyName) \
	OnRep_##PropertyName() \
