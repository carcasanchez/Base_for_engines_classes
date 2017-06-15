#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "Timers.h"

#include "Imgui/imgui_impl_sdl_gl3.h"
#include "OpenGL.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	moduleName = "ModuleEditor";
}

// Destructor
ModuleEditor::~ModuleEditor()
{

}

// Called before render is available
bool ModuleEditor::Init()
{
	bool ret = true;
	LOG("Init editor gui with imgui lib version %s", ImGui::GetVersion());
	//Linking ImGUI and the window
	ImGui_ImplSdlGL3_Init(App->window->GetWindow());

	return ret;
}

bool ModuleEditor::Start()
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());

	OnScreenResize(App->window->GetWindowSize().x, App->window->GetWindowSize().y);

	return true;
}

// Called every draw update
update_status ModuleEditor::PreUpdate()
{
	update_status ret = UPDATE_CONTINUE;

	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());

	ImGuiIO IO = ImGui::GetIO();
	App->input->ignoreMouse = IO.WantCaptureMouse;

	if (IO.WantCaptureKeyboard || IO.WantTextInput)
	{
		App->input->ignoreKeyboard = true;
	}
	else
	{
		App->input->ignoreKeyboard = false;
	}

	return ret;
}

update_status ModuleEditor::Update()
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

update_status ModuleEditor::PostUpdate()
{
	update_status ret = UPDATE_CONTINUE;

	if (IsOpenTestWindow)
	{
		ImGui::ShowTestWindow(&IsOpenTestWindow);
	}

	ret = MenuBar();
	Editor();
	Console();

	return ret;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	ClearConsole();

	ImGui_ImplSdlGL3_Shutdown();

	return true;
}


void ModuleEditor::OnScreenResize(int width, int heigth)
{
}

void ModuleEditor::HandleInput(SDL_Event* event)
{
	ImGui_ImplSdlGL3_ProcessEvent(event);
}

void ModuleEditor::Log(const char* input)
{
	buffer.append(input);
	scrollToBottom = true;
}

void ModuleEditor::ClearConsole()
{
	buffer.clear();
	scrollToBottom = true;
}



// ---- UI with IMGUI viewPort UI -------------------------------------------------------------------

update_status ModuleEditor::MenuBar()
{
	update_status ret = UPDATE_CONTINUE;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene##NewMenuBar"))
			{
				//Do stuff
			}
			if (ImGui::MenuItem("Quit"))
			{
				ret = UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::Checkbox("ImGui TestBox", &IsOpenTestWindow);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Documentation"))
		{
			if (ImGui::MenuItem("MathGeoLib"))
			{
				App->OpenBrowser("http://clb.demon.fi/MathGeoLib/nightly/reference.html");
			}
			if (ImGui::MenuItem("ImGui"))
			{
				App->OpenBrowser("https://github.com/ocornut/imgui");
			}
			if (ImGui::MenuItem("SDL"))
			{
				App->OpenBrowser("https://wiki.libsdl.org/APIByCategory");
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	return ret;
}

void ModuleEditor::Editor()
{
		ImGui::SetNextWindowPos(ImVec2(App->window->GetWindowSize().x - 330, 20.0f));
		ImGui::SetNextWindowSize(ImVec2(330, App->window->GetWindowSize().y - 20));

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		ImGui::Begin("Editor", 0, ImVec2(500, 300), 0.8f, flags);

		ImGui::Text("We'll put stuff in here.\nBy now it's empty, though.\n:(");

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
		if (ImGui::Button("Send stuff to the console"))
		{
			if (n < 13 || n > 40){LOG("Wow! You logged this to the console!");}
			else if (n == 13){LOG("Is this really necessary?");}
			else if (n == 14){LOG("No need to keep pressing me.");}
			else if (n == 15){LOG("Ouch.");}
			else if (n == 16){LOG("I'll stop working now, ok?");}
			else if (n == 17){LOG("Ok. Bye.");}
			else{LOG(" ");}
			n++;
		}

		ImGui::End();
}

void ModuleEditor::Console()
{
		ImGui::SetNextWindowPos(ImVec2(0.0f, App->window->GetWindowSize().y - 200.0f));
		ImGui::SetNextWindowSize(ImVec2(App->window->GetWindowSize().x - 330.0f, 200.0f));

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		ImGui::Begin("Console", 0, ImVec2(500, 300), 0.8f, flags);

		ImColor col = ImColor(0.6f, 0.6f, 1.0f, 1.0f);
		ImGui::PushStyleColor(0, col);

		ImGui::TextUnformatted(buffer.begin());
		ImGui::PopStyleColor();

		if (scrollToBottom)
		{
			ImGui::SetScrollHere(1.0f);
			scrollToBottom = false;
		}

		ImGui::End();
}