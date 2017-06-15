#ifndef __MODULE_UI__
#define __MODULE_UI__

#include "Module.h"
#include "Globals.h"

#include "Math.h"

#include "ImGui\imgui.h"

class GameObject;

class ModuleEditor : public Module
{
public:
	
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	bool CleanUp();

	void OnScreenResize(int width, int heigth);

	void HandleInput(SDL_Event* event);

	void ModuleEditor::Log(const char* input);
	void ClearConsole();
private:

	update_status MenuBar();
	void Editor();
	void Console();

public:
	bool IsOpenTestWindow = false;

private:
	ImGuiTextBuffer buffer;
	bool scrollToBottom;

	int n = 0;
};

#endif