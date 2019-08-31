#include<Siv3D.hpp> // OpenSiv3D v0.3.2
#include<vector>
#include<cmath>

using Math::Pi;

//空間には接頭辞sを、
//視界には接頭辞vを用いる。

//また併せて、
//座標にはxyzを、
//水平角度にはhを、垂直角度にはvを用いる。
//なお水平とはxy軸のある面の事で、垂直とはz軸のある面の事である。

struct pos3 {
	double sx;
	double sy;
	double sz;
	//pos3* connect;
};

struct {
	pos3 sPos;
	double shView, svView;
	double shMax, svMax;
	//pos3 headpos;

	void shAdd(double val) { shView += val * Pi; }
	void svAdd(double val) { svView += val * Pi; }
} me = { { 0,0,1 } ,0 ,0 ,Pi / 8 ,Pi / 8 };

Array<pos3> objlist;
void generatePoints();
void control();
void drawMap();
void draw();

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

	//球
	for (double i = 0;i < Pi;i += 0.1) {
		for (double j = 0;j < 2_pi;j += 0.1) {
			constexpr int sxCenter = -2, syCenter = -2;
			objlist.push_back({ sxCenter + sin(j) * sin(i),syCenter + cos(j) * sin(i),cos(i) });
		}
	}

	//円
	for (double i = 0;i < 2_pi;i += 0.05) {
		constexpr int sxCenter = 3, syCenter = 3;
		objlist.push_back({ sxCenter + sin(i),syCenter + cos(i),0 });
	}

	//直線
	for (int i = 0;i < 20;i += 2) {
		objlist.push_back({ (double)i,2,1 });
	}
	for (int i = 1;i < 20;i += 1) {
		objlist.push_back({ (double)i,0,0 });
	}
}

void control() {
	double sxForward = cos(me.shView) * 0.1;	//前進時のX軸移動量
	double syForward = sin(me.shView) * 0.1;	//前進時のY軸移動量
	const double shViewMove = 0.02;	//視覚移動量

	if (KeyW.pressed()) {
		me.sPos.sx += sxForward;
		me.sPos.sy += syForward;
	}
	if (KeyS.pressed()) {
		me.sPos.sx -= sxForward;
		me.sPos.sy -= syForward;
	}
	if (KeyA.pressed()) {
		if (KeyShift.pressed()) {
			me.sPos.sx -= syForward;
			me.sPos.sy += sxForward;
		}
		else {
			me.shAdd(shViewMove);
		}
	}
	if (KeyD.pressed()) {
		if (KeyShift.pressed()) {
			me.sPos.sx += syForward;
			me.sPos.sy -= sxForward;
		}
		else {
			me.shAdd(-shViewMove);
		}
	}
}

void drawMap() {
	constexpr int center = 50;	//原点(XY共用)
	constexpr int  magni = 3;	//拡大率
	//マップ背景色
	Rect(0, 0, 200, 200).draw(Palette::White);
	//自分の視界
	Circle(center + me.sPos.sx * magni, center - me.sPos.sy * magni, 50).drawPie(0.5_pi - me.shView - me.shMax, me.shMax * 2, ColorF(0.0, 0.0, 1.0, 0.5));
	//点の位置
	for (auto& obj : objlist) {
		Circle(center + obj.sx * magni, center - obj.sy * magni, 1).draw(Palette::Red);
	}
}

void draw() {
	for (auto& obj : objlist) {
		double sxDiff = (obj.sx - me.sPos.sx);
		double syDiff = (obj.sy - me.sPos.sy);if (syDiff == 0)syDiff = 0.01;
		double szDiff = (obj.sz - me.sPos.sz);
		double sxyDiff = sqrt(pow(sxDiff, 2) + pow(syDiff, 2));

		double shObjByMe = atan(syDiff / sxDiff);
		double svObjByMe = atan(szDiff / sxyDiff);

		if (sxDiff < 0) {
			shObjByMe += Pi;
		}

		double shObjInView = shObjByMe - me.shView;
		double svObjInView = svObjByMe - me.svView;

		double vxObjInView = (Window::Width() / 2) + (Window::Width() / 2) * -(shObjInView / me.shMax);
		double vyObjInView = (Window::Height() / 2) + (Window::Height() / 2) * -(svObjInView / me.svMax);

		Circle(vxObjInView, vyObjInView, 5).draw();
	}
}
