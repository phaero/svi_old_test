#ifndef COMPONENT_H_INCLUDES__
#define COMPONENT_H_INCLUDES__

typedef struct svi_component_handle_1 {
	uint64_t handle;
} svi_component_handle_1T;

typedef void (*svi_component_fp_1T)( svi_component_handle_1T handle );

struct svi_interface_1 {
	const char* name;
	const uint32_t version;
};

struct svi_component_1 {
	struct svi_interface_1 base;

	svi_component_fp_1T* start;
	svi_component_fp_1T* stop;

	struct svi_interface_1 **impl;
	struct svi_interface_1 **deps;
};

struct svi_portal_1 {
	void register_component( svi_library_handle_1T handle, struct svi_component_1 *component, struct svi_component_handle_1T **comp_handle );
};

typedef struct svi_library_handle_1 {
	uint64_t handle;
} svi_library_handle_1T;

void svi_library_init( svi_library_handle_1T handle, struct svi_portal_1 *portal );
void svi_library_fin( svi_library_handle_1T handle, struct svi_portal_1 *portal );

#endif

