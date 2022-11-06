#include <vector>
#include <numeric>
#include <functional> 
#include <algorithm>
#include <iostream>

class Dash
{
public:
	// элемеет паттерна
	// l - длина элемента
	// ls - тип элемента, 1 - отрезок или 0 - зазор 
	class DashElement
	{
	public:
		float l;
		bool ls;
	};
	// ----------------------------
	// Конструктор
	// ----------------------------
	Dash();
	Dash(std::vector<float> pattern);
	Dash(std::vector<DashElement> pattern);
	// ----------------------------
	// деструктор
	// ----------------------------
	~Dash();
	// ----------------------------
	// 
	// ----------------------------
	float getTotalLength();
	// ----------------------------
	// 
	// ----------------------------
	float getOffst();
	// ----------------------------
	// установка исходного нормализованного паттерна. 
	// Для удобства расчетов, длина паттерна приведена к 1.
	// Параметр отрезок/зазор устанавливается
	// автоматически, чередованием, начиная с
	// отрезка.
	// Также вычисляется дляна исходного паттерна.
	// ----------------------------
	void setPattern(std::vector<float> pattern);
	// ----------------------------
	// установка рабочего нормированного паттерна
	// с ручной установкой типа элементов
	// ----------------------------
	void setPattern(std::vector<DashElement> pattern);
	// ----------------------------
	// вырезаем кусочкк из паттерна
	// координаты начала и конца нормированы
	// в интервале [0:1]
	// ----------------------------
	void samplePattern(float start, float end, std::vector<DashElement>& pattern);
	// ----------------------------
	// float shift - величина сдвига в нормализованном виде [-1:+1]
	// std::vector<float>& pattern - результат
	// ----------------------------
	void shiftPattern(float offset, std::vector<DashElement>& pattern);
	// ----------------------------
	// 
	// ----------------------------
	void consolidatePattern(std::vector<DashElement>& pattern);
	private:
	// ----------------------------
	// 
	// ----------------------------
	const float eps = FLT_EPSILON;
	float totalLength;
	float offset;
	std::vector<DashElement> normalized_pattern;
};
// 
float makeDashedLineSegment(
	// вход
	float x1,// координата x начальной точки
	float y1,// координата y начальной точки
	float x2,// координата x конечной точки
	float y2,// координата y конечной точки
	std::vector<float>& dash, // паттерн 
	float dash_offset, // смещение фазы [-1:1]
	// результат
	std::vector<float>& xp,// x координаты точек для отрисовки отрезков.
	std::vector<float>& yp // y координаты точек для отрисовки отрезков.
);

void makeDashedPolyline(
	std::vector<float>& pt_nodes_x,	// (вход) вектор x-координат узлоых точек отрезков полилинии
	std::vector<float>& pt_nodes_y,	// (вход) вектор y-координат узлоых точек отрезков полилинии
	std::vector<float> dash, //(вход) паттерн
	float dash_offset, //(вход) смещение фазы паттерна [-1:+1]
	std::vector<float>& xp,//(выход) вектор x-координат для отрисовки полилинии
	std::vector<float>& yp,//(выход) вектор y-координат для отрисовки полилинии
	float & r //(вход/выход) остаток, нужен для отписовки соединенных последовательно линий.
);
