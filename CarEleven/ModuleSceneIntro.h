#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	//Render primitives
	void RenderPrimitives();

	//Death sensor
	void CreateDeathSensor();

	//Rotating constraint
	void CreateLevelConstraints();
	void UpdateRotatingConstraints();
	void CreateRotatingConstraint(int index, vec3 size, vec3 pos, float velocity = 3.0f, float mass = 100000.0f, Color color = { 0,255,0,255 });

	//Ring
	void DefineRing();

	//Functions to create physics primitives
	Cube CreateCube(vec3 size, vec3 pos, Color color, float mass = 0.0f, bool draw = true, bool isPhysicsObject = true, bool rotate = false, float angleToRot = 0.0f, vec3  axisToRot = (0.0f, 1.0f, 0.0f));
	Cylinder CreateCylinder(float radius, float height, vec3 pos, Color color, float mass = 0.0f, bool draw = true, bool isPhysicsObject = true, bool rotate = false, float angleToRot = 0.0f, vec3  axisToRot = (0.0f, 1.0f, 0.0f));
	Sphere CreateSphere(float radius, vec3 pos, Color color, float mass = 0.0f, bool draw = true, bool isPhysicsObject = true, bool rotate = false, float angleToRot = 0.0f, vec3  axisToRot = (0.0f, 1.0f, 0.0f));

public:

	//Death sensor
	PhysBody3D* deathSensorPB;

	Cube rotConstraint[4];
	PhysBody3D* rotConstraintPB[4];

	//Objects to render
	p2List<Cube> cubesToRender;
	p2List<Cylinder> cylindersToRender;
	p2List<Sphere> spheresToRender;

};
