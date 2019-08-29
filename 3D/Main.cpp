#include<Siv3D.hpp> // OpenSiv3D v0.3.2
#include<vector>
#include<cmath>

using namespace std;

constexpr float PI = 3.14;

struct pos3 {
	double x;
	double y;
	double z;
};

struct {
	pos3 pos;
	double angleX;
	double angleY;
	//pos3 headpos;
	double maxAngle;

	void addAngleX(double val) { angleX += val * PI; }
	void addAngleY(double val) { angleY += val * PI; }
} me = { { 0,0,1 } ,0 * PI ,0 * PI ,PI / 8 };

std::vector<pos3> objlist;
void generatePoints();
void control();
void draw();
void drawMap();

void Main()
{
	generatePoints();

	while (System::Update()) {
		ClearPrint();
		control();
		drawMap();
		draw();
	}
}

void generatePoints() {
	for (int i = 0;i < 20;i += 2) {
		objlist.push_back({ (double)i,2,1 });
	}
	for (int i = 1;i < 20;i += 2) {
		objlist.push_back({ (double)i,0,0 });
	}
}

void control() {
	double forwardX = cos(me.angleX) * 0.1;
	double forwardY = sin(me.angleX) * 0.1;

	if (KeyW.pressed()) {
		me.pos.x += forwardX;
		me.pos.y += forwardY;
	}
	if (KeyS.pressed()) {
		me.pos.x -= forwardX;
		me.pos.y -= forwardY;
	}
	if (KeyA.pressed()) {
		if (KeyShift.pressed()) {
			me.pos.x -= forwardY;
			me.pos.y += forwardX;
		}
		else {
			me.addAngleX(0.02);
		}
	}
	if (KeyD.pressed()) {
		if (KeyShift.pressed()) {
			me.pos.x += forwardY;
			me.pos.y -= forwardX;
		}
		else {
			me.addAngleX(-0.02);
		}
	}
}

void drawMap() {
	Rect(0, 0, 300, 300).draw();
	Circle(150 + me.pos.x * 3, 150 - me.pos.y * 3, 50).drawPie(PI / 2 - me.angleX - me.maxAngle, me.maxAngle * 2, ColorF(0.0, 0.0, 1.0, 0.5));

	for (auto& obj : objlist) {
		Circle(150 + obj.x * 3, 150 - obj.y * 3, 1).draw(Palette::Red);
	}
}

void draw() {
	for (auto& obj : objlist) {
		double diffX = (obj.x - me.pos.x);
		double diffY = (obj.y - me.pos.y);if (diffY == 0)diffY = 0.01;
		double diffZ = (obj.z - me.pos.z);
		double diffXY = sqrt(pow(diffX, 2) + pow(diffY, 2));

		double viewAngleX = atan(diffY / diffX) - me.angleX;
		double viewAngleY = atan(diffZ / diffXY) - me.angleY;

		if (diffX < 0) {
			viewAngleX += PI;
		}

		double posinViewX = (Window::Width() / 2) + (Window::Width() / 2) * -(viewAngleX / me.maxAngle);
		double posinViewY = (Window::Height() / 2) + (Window::Height() / 2) * -(viewAngleY / me.maxAngle);

		Circle(posinViewX, posinViewY, 5).draw();
	}
}
