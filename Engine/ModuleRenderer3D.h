#ifndef __MODULE_RENDER_3D__
#define __MODULE_RENDER_3D__

#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include <map>

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

private:
};

#endif