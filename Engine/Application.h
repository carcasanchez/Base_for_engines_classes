#ifndef __APP__
#define __APP__

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"

#include <vector>
#include <list>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleRenderer3D;
class ModuleEditor;
class ModuleFileSystem;
class TimerManager;

struct viewPort;

#include "MathGeoLib\include\MathGeoLibFwd.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleFileSystem* fs;

	ModuleRenderer3D* renderer3D;

	ModuleEditor* Editor;

	float ms_frame[EDITOR_FRAME_SAMPLES];
	float framerate[EDITOR_FRAME_SAMPLES];
	int maxFPS = 0;

	TimerManager* timers;

private:

	PerfTimer	ms_timer;
	std::vector<Module*> list_modules;
	int frameCount = 0;
	Timer	FPS_Timer;
	float FrameTime = -1.0f;
	int previous_maxFPS = maxFPS;
	bool gameRunning = false;

	std::string title;
	std::string organisation;

public:

	Application();
	~Application();

	Timer totalTimer;
	bool Init();
	update_status Update();
	bool CleanUp();

	bool OpenBrowser(const char* link);
	void Log(char* str);

	const char* GetOrganization();
	const char* GetTitle();

	void OnScreenResize(int width, int heigth);
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif