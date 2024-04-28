#include "Macros.h"
#include "KeyCodes.h"
#include "EventDispatcher.h"
using namespace Orchis;

OC_EXPORT void OrchisEventsRegisterKeyDownEvent(Key key)
{
	EventDispatcher::Dispatch<KeyDownEvent>({ key });
}

OC_EXPORT void OrchisEventsRegisterKeyUpEvent(Key key)
{
	EventDispatcher::Dispatch<KeyUpEvent>({ key });
}