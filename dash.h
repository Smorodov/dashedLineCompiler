#include <vector>
#include <numeric>
#include <functional> 
#include <algorithm>
#include <iostream>

class Dash
{
public:
	// ������� ��������
	// l - ����� ��������
	// ls - ��� ��������, 1 - ������� ��� 0 - ����� 
	class DashElement
	{
	public:
		float l;
		bool ls;
	};
	// ----------------------------
	// �����������
	// ----------------------------
	Dash();
	Dash(std::vector<float> pattern);
	Dash(std::vector<DashElement> pattern);
	// ----------------------------
	// ����������
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
	// ��������� ��������� ���������������� ��������. 
	// ��� �������� ��������, ����� �������� ��������� � 1.
	// �������� �������/����� ���������������
	// �������������, ������������, ������� �
	// �������.
	// ����� ����������� ����� ��������� ��������.
	// ----------------------------
	void setPattern(std::vector<float> pattern);
	// ----------------------------
	// ��������� �������� �������������� ��������
	// � ������ ���������� ���� ���������
	// ----------------------------
	void setPattern(std::vector<DashElement> pattern);
	// ----------------------------
	// �������� ������� �� ��������
	// ���������� ������ � ����� �����������
	// � ��������� [0:1]
	// ----------------------------
	void samplePattern(float start, float end, std::vector<DashElement>& pattern);
	// ----------------------------
	// float shift - �������� ������ � ��������������� ���� [-1:+1]
	// std::vector<float>& pattern - ���������
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
	// ����
	float x1,// ���������� x ��������� �����
	float y1,// ���������� y ��������� �����
	float x2,// ���������� x �������� �����
	float y2,// ���������� y �������� �����
	std::vector<float>& dash, // ������� 
	float dash_offset, // �������� ���� [-1:1]
	// ���������
	std::vector<float>& xp,// x ���������� ����� ��� ��������� ��������.
	std::vector<float>& yp // y ���������� ����� ��� ��������� ��������.
);

void makeDashedPolyline(
	std::vector<float>& pt_nodes_x,	// (����) ������ x-��������� ������ ����� �������� ���������
	std::vector<float>& pt_nodes_y,	// (����) ������ y-��������� ������ ����� �������� ���������
	std::vector<float> dash, //(����) �������
	float dash_offset, //(����) �������� ���� �������� [-1:+1]
	std::vector<float>& xp,//(�����) ������ x-��������� ��� ��������� ���������
	std::vector<float>& yp,//(�����) ������ y-��������� ��� ��������� ���������
	float & r //(����/�����) �������, ����� ��� ��������� ����������� ��������������� �����.
);
