#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "imGUI\imgui.h"

#include "OpenGL.h"

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	moduleName = "ModuleRenderer3D";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->GetWindow());
	if (context == nullptr)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		LOG("Glew library could not init %s\n", glewGetErrorString(err));
		ret = false;
	}
	else
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	if (ret == true)
	{
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(10.0f);

		//Initialize clear color
		glClearColor(0.78f, 0.81f, 0.84f, 1.f);

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		light.ref = GL_LIGHT0;
		light.ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		light.diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		light.SetPos(5, 10, 0);
		light.Init();
		light.Active(true);

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading

		glLineWidth(1.0f);
	}

	return ret;
}

bool ModuleRenderer3D::Start()
{
	bool ret = true;
	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Check for error
	
	while (GLenum error = glGetError() != GL_NO_ERROR)
	{
		LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
		ret = false;
	}

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate()
{
	light.Render();

#if USE_EDITOR
	ImGui::Render();
#endif

	SDL_GL_SwapWindow(App->window->GetWindow());
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::DrawLine(float3 a, float3 b, float4 color)
{
	if (usingLights)
	{
		glDisable(GL_LIGHTING);
	}
	glColor4f(color.x, color.y, color.z, color.w);

	glBegin(GL_LINES);

	glVertex3fv(a.ptr()); glVertex3fv(b.ptr());

	glEnd();

	if (usingLights)
	{
		glEnable(GL_LIGHTING);
	}
}

void ModuleRenderer3D::DrawLocator(float4x4 transform, float4 color)
{
	if (usingLights)
	{
		glDisable(GL_LIGHTING);
	}
	glPushMatrix();
	glMultMatrixf(transform.ptr());

	glColor4f(color.x, color.y, color.z, color.w);

	glBegin(GL_LINES);

	glVertex3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.0f, -1.0f);
	//Arrow indicating forward
	glVertex3f(0.0f, 0.0f, 1.0f); glVertex3f(0.1f, 0.0f, 0.9f);
	glVertex3f(0.0f, 0.0f, 1.0f); glVertex3f(-0.1f, 0.0f, 0.9f);

	glEnd();

	if (usingLights)
	{
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
}

void ModuleRenderer3D::DrawLocator(float3 position, float4 color)
{
	App->renderer3D->DrawLocator((float4x4::FromTRS(position, float4x4::identity, float3(1, 1, 1))).Transposed(), color);
}