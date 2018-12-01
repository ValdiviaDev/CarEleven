#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(20.0f, 80.0f, -5.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	//Death sensor
	CreateDeathSensor();

	//Ring
	DefineRing();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	UpdateRing();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2 == (PhysBody3D*)App->player->car01 && body1 == deathSensorPB)
		App->player->car01->SetPos(0, 12, 10);

	if (body2 == (PhysBody3D*)App->player->car02 && body1 == deathSensorPB)
		App->player->car02->SetPos(0, 12, -10);


	//LOG("Hit!");
}

void ModuleSceneIntro::CreateDeathSensor()
{
	deathSensor.size = vec3(200.0f, 5.0f, 200.0f);
	deathSensor.SetPos(0.0f, -10.0f, 0.0f);

	deathSensorPB = App->physics->AddBody(deathSensor, 0.0f);
	deathSensorPB->SetAsSensor(true);
	deathSensorPB->collision_listeners.add(this);
}

void ModuleSceneIntro::DefineRing()
{
	//Ring printing in the form of a cylinder
	ring.radius = 40.0f;
	ring.height = 80.0f;
	ring.SetPos(0.0f, - ring.height/2, 0.0f);
	ring.SetRotation(90.0f, vec3(0.0f, 0.0f, 1.0f));
	
	ring.color = { 0,255,255, 255 };

	//Physbody in the form of a cube
	cubeRing = Cube(80.0f, 0.0f, 80.0f);
	ringPB = App->physics->AddBody(cubeRing, 0.0f);
	cubeRing = Cube(80.0f, -1.0f, 80.0f);
	cubeRing.color = { 255,0,0,255 };
}

void ModuleSceneIntro::UpdateRing()
{
	ring.Render();
	cubeRing.Render();
}

