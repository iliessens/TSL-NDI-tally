#include "NDITally.h"

#include <cstdio>

NDITally::NDITally(NDILib_loader* loader, string source_name) {
	this->name = source_name;
	this->p_NDILib = loader->get_NDI();

	NDIlib_source_t source;
	source.p_ndi_name = source_name.c_str(); // Must be exactly right

	// Create source settings
	NDIlib_recv_create_v3_t recv_settings;
	recv_settings.bandwidth = NDIlib_recv_bandwidth_metadata_only;
	recv_settings.source_to_connect_to = source;

	// We now have at least one source, so we create a receiver to look at it.
	pNDI_recv = p_NDILib->recv_create_v3(&recv_settings);
	if (!pNDI_recv) throw "error creating receiver";

	printf("Connecting to %s \n", source.p_ndi_name);
}

NDITally::~NDITally() {
	// Destroy the receiver
	p_NDILib->recv_destroy(pNDI_recv);
}

//returns FALSE if we are not currently connected to anything.
bool NDITally::setState(bool program, bool preview)
{
	NDIlib_tally_t tally_state;
	tally_state.on_program = program;
	tally_state.on_preview = preview;

	return p_NDILib->recv_set_tally(pNDI_recv, &tally_state);
}

string NDITally::get_name() {
	return this->name;
}