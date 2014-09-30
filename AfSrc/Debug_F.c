
#include "Includes_F.h"

#include "Debug_F.h"

static UART_HANDLE _DbgHandler = NULL;

void		DBG_SetDebugHandle(UART_HANDLE uH)
{
	_DbgHandler = uH;
}