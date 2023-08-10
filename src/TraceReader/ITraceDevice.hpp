#ifndef _ITRACEDEVICE_HPP
#define _ITRACEDEVICE_HPP

#include <stdint.h>

class ITraceDevice
{
   public:
	virtual ~ITraceDevice() = default;
	virtual bool startTrace(uint32_t coreFrequency, uint32_t traceFrequency) = 0;
	virtual bool stopTrace() = 0;
	virtual uint32_t readTraceBuffer(uint8_t* buffer, uint32_t size) = 0;
};

#endif