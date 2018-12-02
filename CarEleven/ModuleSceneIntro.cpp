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

	App->camera->Move(vec3(60.0f, 70.0f, -5.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	//Death sensor
	CreateDeathSensor();

	//Ring
	DefineRing();

	//Constraints
	CreateLevelConstraints();

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
	UpdateRotatingConstraints();

	//Render everything on scene
	RenderPrimitives();



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

void ModuleSceneIntro::RenderPrimitives()
{
	//Render cubes
	for (p2List_item<Cube>* item = cubesToRender.getFirst(); item; item = item->next)
		item->data.Render();

	//Render cylinders
	for (p2List_item<Cylinder>* item = cylindersToRender.getFirst(); item; item = item->next)
		item->data.Render();

	//Render spheres
	for (p2List_item<Sphere>* item = spheresToRender.getFirst(); item; item = item->next)
		item->data.Render();

}

void ModuleSceneIntro::CreateDeathSensor()
{
	Cube deathSensor(600.0f, 5.0f, 600.0f);
	deathSensor.SetPos(0.0f, -10.0f, 0.0f);

	deathSensorPB = App->physics->AddBody(deathSensor, 0.0f);
	deathSensorPB->SetAsSensor(true);
	deathSensorPB->collision_listeners.add(this);
}

void ModuleSceneIntro::CreateLevelConstraints()
{
	//Putting every PhisBody3D* null just in case
	for (int i = 0; i < 4; i++)
		rotConstraintPB[i] = nullptr;

	//Level rotating constraitns
	CreateRotatingConstraint(0, vec3(2.0f, 1.0f, 20.0f), vec3(-30.0f, 2.0f, 30.0f), 5.0f, 100000.0f, { 0,0,255,255 });
	CreateRotatingConstraint(1, vec3(2.0f, 1.0f, 20.0f), vec3(-30.0f, 2.0f, -30.0f), -5.0f, 100000.0f, { 0,0,255,255 });
	CreateRotatingConstraint(2, vec3(2.0f, 1.0f, 20.0f), vec3(30.0f, 2.0f, 30.0f), -5.0f, 100000.0f, { 0,0,255,255 });
	CreateRotatingConstraint(3, vec3(2.0f, 1.0f, 20.0f), vec3(30.0f, 2.0f, -30.0f), 5.0f, 100000.0f, { 0,0,255,255 });
}

void ModuleSceneIntro::CreateRotatingConstraint(int index, vec3 size, vec3 pos, float velocity, float mass, Color color)
{
	rotConstraint[index] = CreateCube(vec3(size), vec3(pos), color, 0.0f, false, false);
	rotConstraintPB[index] = App->physics->AddBody(rotConstraint[index], mass);

	Cylinder constraintRotator = CreateCylinder(size.z, 10.0f, vec3(size.x, size.y + 100.0f, size.z), color, 0.0f, false, false, true, 90.0f, vec3(0.0f, 0.0f, 1.0f));
	PhysBody3D* constraintRotatorPB = App->physics->AddBody(constraintRotator, mass);
	
	rotConstraintPB[index]->GetBody()->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
	constraintRotatorPB->GetBody()->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));

	App->physics->AddConstraintHinge(*rotConstraintPB[index], *constraintRotatorPB, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), true, true, velocity);
}


void ModuleSceneIntro::UpdateRotatingConstraints()
{
	for (int i = 0; i < 4; i++) {
		if (rotConstraintPB[i] != nullptr) {
			rotConstraintPB[i]->GetTransform(&rotConstraint[i].transform);
			rotConstraint[i].Render();
		}
	}
}

void ModuleSceneIntro::DefineRing()
{
	//Ring printing in the form of a cylinder
	CreateCylinder(40.0f, 80.f, vec3(0.0f, -40.f, 0.0f), { 0,255,255, 255 }, 0.0f, true, false, true, 90.0f, vec3(0.0f, 0.0f, 1.0f));

	//Physbody in the form of a cube
	CreateCube(vec3(80.0f, 80.0f, 80.0f), vec3(0.0f, -40.1f, 0.0f), { 255,0,0,255 });
}

Cube ModuleSceneIntro::CreateCube(vec3 size, vec3 pos, Color color, float mass, bool draw, bool isPhysicsObject, bool rotate, float angleToRot, vec3  axisToRot)
{
	Cube cube(size.x, size.y, size.z);
	cube.SetPos(pos.x, pos.y, pos.z);
	cube.color = color;

	if (rotate)
		cube.SetRotation(angleToRot, vec3(axisToRot.x, axisToRot.y, axisToRot.z));

	if (draw)
		cubesToRender.add(cube);

	if (isPhysicsObject) 
		App->physics->AddBody(cube, mass);

	return cube;
}

Cylinder ModuleSceneIntro::CreateCylinder(float radius, float height, vec3 pos, Color color, float mass, bool draw, bool isPhysicsObject, bool rotate, float angleToRot, vec3 axisToRot)
{
	Cylinder cylinder(radius, height);
	cylinder.SetPos(pos.x, pos.y, pos.z);
	cylinder.color = color;

	if(rotate)
		cylinder.SetRotation(angleToRot, vec3(axisToRot.x, axisToRot.y, axisToRot.z));

	if (draw)
		cylindersToRender.add(cylinder);

	if (isPhysicsObject)
		App->physics->AddBody(cylinder, mass);

	return cylinder;
}

Sphere ModuleSceneIntro::CreateSphere(float radius, vec3 pos, Color color, float mass, bool draw, bool isPhysicsObject, bool rotate, float angleToRot, vec3 axisToRot)
{
	Sphere sphere(radius);
	sphere.SetPos(pos.x, pos.y, pos.z);
	sphere.color = color;

	if (rotate)
		sphere.SetRotation(angleToRot, vec3(axisToRot.x, axisToRot.y, axisToRot.z));

	if (draw)
		spheresToRender.add(sphere);

	if(isPhysicsObject)
		App->physics->AddBody(sphere, mass);

	return sphere;
}

