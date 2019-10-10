#pragma once

#include "pdu.h"
#include <functional>


class BTRadio {
	
private:
	void InitClock();
	
public:
	void Init();
	void ReadPacket();
};

