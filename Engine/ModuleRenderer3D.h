#ifndef __MODULE_RENDER_3D__
#define __MODULE_RENDER_3D__

#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include <map>

#define MAX_LIGHTS 8

struct Mesh_RenderInfo;
struct viewPort;
class Camera;

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

	void DrawLine(float3 a, float3 b, float4 color = float4(0.1f, 0.58f, 0.2f, 1.0f));
	void DrawLocator(float4x4 transform = float4x4::identity, float4 color = float4(0.1f, 0.58f, 0.2f, 1.0f));
	void DrawLocator(float3 position, float4 color = float4(0.1f, 0.58f, 0.2f, 1.0f));

	Light light;

private:
	SDL_GLContext context;

	//Only for read access
	bool usingLights = true;
};

#endif