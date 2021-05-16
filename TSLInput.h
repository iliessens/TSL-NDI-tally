#pragma once

#include "TallyState.h"

class TSLInput {
public:
	TSLInput();
	~TSLInput();

	// Blocking wait for next message
	TallyState listen();

private:
	TallyState parse_packet(char* buf);
	SOCKET s;
};