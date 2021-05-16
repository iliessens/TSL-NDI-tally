#include <vector>
#include <stdio.h>

#include "NDIlib_loader.h"
#include "TSLInput.h"
#include "SourceMapper.h"
#include "TallyState.h"
#include "NDITally.h"

int main(int argc, char* argv[])
{	
	try {
		NDILib_loader loader;
		SourceMapper mapper(&loader);

		TSLInput tsl;

		while (1) {
			TallyState state = tsl.listen();
			NDITally* tally = mapper.id_to_source(state.id);
			tally->setState(state.program, state.preview);
		}
	}
	catch (std::exception& e) {
		printf("Exception encountered!\n");
		printf("Message: %s\n", e.what());
		printf("Application exiting...\n");
	}

	// Finished
	return 0;
}
