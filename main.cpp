#include <blend2d.h>
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
	Dash()
	{
		// ����� ��������� ��������
		totalLength = 1;
		// �������� ���� [-1:1]
		offset = 0;
	}
	Dash(std::vector<float> pattern)
	{
		setPattern(pattern);
		// �������� ���� [-1:1]
		offset = 0;
	}
	Dash(std::vector<DashElement> pattern)
	{
		setPattern(pattern);
		// �������� ���� [-1:1]
		offset = 0;
	}
	// ----------------------------
	// ����������
	// ----------------------------
	~Dash()
	{
		normalized_pattern.clear();
	}
	// ----------------------------
	// 
	// ----------------------------
	float getTotalLength()
	{
		return totalLength;
	}
	// ----------------------------
	// 
	// ----------------------------
	float getOffst()
	{
		return offset;
	}
	// ----------------------------
	// ��������� ��������� ���������������� ��������. 
	// ��� �������� ��������, ����� �������� ��������� � 1.
	// �������� �������/����� ���������������
	// �������������, ������������, ������� �
	// �������.
	// ����� ����������� ����� ��������� ��������.
	// ----------------------------
	void setPattern(std::vector<float> pattern)
	{
		// ������� ��������������� �������
		normalized_pattern.clear();
		// ������� ����� ��������� ��������
		totalLength = 0;
		for (auto e : pattern)
		{
			totalLength += e;
		}
		// ���� ����� �������, �������
		if (this->totalLength < eps)
		{	
			std::cout << "pattern length=0" << std::endl;
			return;
		}
		// �������� �������/������
		bool f = true;
		for (auto& e : pattern)
		{
			normalized_pattern.push_back({ e / totalLength,f });
			f = !f;
		}
	}

	// ----------------------------
	// ��������� �������� �������������� ��������
	// � ������ ���������� ���� ���������
	// ----------------------------
	void setPattern(std::vector<DashElement> pattern)
	{
		normalized_pattern.clear();
		// ��������� �������� ����� ��������
		totalLength = 0;		
		for (auto e : pattern)
		{
			totalLength += e.l;
		}
		// �������, ���� ����� �������
		if (totalLength < eps)
		{
			std::cout << "pattern length=0" << std::endl;
			return;
		}
		// ��������� �������
		for (auto& e : pattern)
		{
			normalized_pattern.push_back({ e.l / totalLength,e.ls });
		}
	}
	// ----------------------------
	// �������� ������� �� ��������
	// ���������� ������ � ����� �����������
	// � ��������� [0:1]
	// ----------------------------
	void samplePattern(float start, float end, std::vector<DashElement>& pattern)
	{
		if (start < 0)
		{
			std::cout << "sampler: start < 0 " << std::endl;
			start = 0;
		}
		if (start > 1)
		{
			std::cout << "sampler: start > 1 " << std::endl;
			start = 1;
		}
		if (end < 0)
		{
			std::cout << "sampler: end < 0 " << std::endl;
			end = 0;
		}
		if (end > 1)
		{
			std::cout << "sampler: end > 0 " << std::endl;
			start = 1;
		}
		if (start > end)
		{
			std::cout << "sampler: start > end " << std::endl;
			std::swap(start, end);
		}
		// ����������� �������� ��������
		// ���������� �������� � ��������� [0:1]
		// 0 - ������ ��������
		// 1 - ����� ��������
		pattern.clear();
		int n = 0;
		float sum = 0;
		for (; n < normalized_pattern.size(); ++n)
		{
			DashElement e = normalized_pattern[n];
			float elStart = sum;
			float elEnd = sum + e.l;
			// ��������� �������� ������� ����� ������ ������ ��������
			// --|-----|--
			if (elStart <= start && elEnd >= end)
			{
				pattern.push_back({ e.l - (start - elStart) - (elEnd - end),e.ls });
				break;
			}
			// ������� ������� ����� ����� ������� � ������
			// �������������� ���������
			// | ------   |
			else if (elStart >= start && elEnd < end)
			{
				pattern.push_back({ e.l, e.ls });
			}
			// --|-----   |
			else if (elStart < start && elEnd < end)
			{
				pattern.push_back({ elEnd - start, e.ls });
			}
			// |   -----|--
			else if (elStart > start && elEnd > end)
			{
				pattern.push_back({ end - elStart, e.ls });
				break;
			}
			sum += e.l;
		}
	}
	// ----------------------------
	// float shift - �������� ������ � ��������������� ���� [-1:+1]
	// std::vector<float>& pattern - ���������
	// ----------------------------
	void shiftPattern(float offset, std::vector<DashElement>& pattern)
	{
		// ����������
		offset = -offset;
		this->offset = offset;
		// ������� ������ ����������
		pattern.clear();
		// ����� �����
		if (offset < 0)
		{
			// ������� ����� �����
			offset = offset - ceil(offset);
		}
		// ����� ������
		else
		{
			// ������� ����� �����
			offset = offset - floor(offset);
		}
		// ----------------------------
		// ����� �����
		// ----------------------------
		if (fabs(offset) > eps)
		{
			int n = 0;
			int split = -1;
			float sum = offset > 0 ? offset : 1 + offset;
			for (DashElement e : normalized_pattern)
			{
				sum -= e.l;
				if (sum < 0 && split == -1)
				{
					pattern.push_back({ e.l + sum , e.ls });
					pattern.push_back({ -sum , e.ls });
					++n;
					split = n;
					continue;
				}
				pattern.push_back(e);
				++n;
			}
			std::vector<DashElement> part1;
			std::vector<DashElement> part2;
			part1.assign(pattern.begin(), pattern.begin() + split);
			part2.assign(pattern.begin() + split, pattern.end());
			pattern.clear();
			pattern.insert(pattern.end(), part2.begin(), part2.end());
			pattern.insert(pattern.end(), part1.begin(), part1.end());
			part1.clear();
			part2.clear();
		}
		else
		{
			pattern.assign(normalized_pattern.begin(), normalized_pattern.end());
		}
		consolidatePattern(pattern);
	}
	// ----------------------------
	// 
	// ----------------------------
	void consolidatePattern(std::vector<DashElement>& pattern)
	{
		// ��������� �������� �������� 
		// � ���������� �����
		for (int i = 0; i < pattern.size() - 1; ++i)
		{
			if (pattern[i].ls == pattern[i + 1].ls)
			{
				pattern[i].l += pattern[i + 1].l;
				pattern.erase(pattern.begin() + i);
				--i;
			}
		}
	}
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
)
{
	// ��������� �������
	std::vector<Dash::DashElement> shifted_dash;
	// ��� ��������� ������� �������� ����� �������
	// (� ������� �� ���������� ����� ����� ��������� ��������)
	std::vector<Dash::DashElement> sampled_dash;
	// ������ ��� ������ � ����������
	Dash lineDash;
	lineDash.setPattern(dash);
	lineDash.shiftPattern(dash_offset, shifted_dash);
	float originalPatternLength = lineDash.getTotalLength();
	float liineSegmentLength = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	if (liineSegmentLength < FLT_EPSILON)
	{
		return 0;
	}
	int times = floor(liineSegmentLength / originalPatternLength);
	float reminder = (liineSegmentLength - times * originalPatternLength) / originalPatternLength;
	float ortx = (x2 - x1) / liineSegmentLength;
	float orty = (y2 - y1) / liineSegmentLength;
	float sum = 0;
	for (int j = 0; j < times; ++j)
	{
		for (int i = 0; i < shifted_dash.size(); ++i)
		{
			if (shifted_dash[i].ls == 1)
			{
				float xstart = x1 + sum * ortx * originalPatternLength;
				float ystart = y1 + sum * orty * originalPatternLength;
				xp.push_back(xstart + sum * ortx * originalPatternLength);
				xp.push_back(xstart + shifted_dash[i].l * originalPatternLength * ortx);
				
				yp.push_back(ystart);
				yp.push_back(ystart + shifted_dash[i].l * originalPatternLength * orty);
			}
			sum += shifted_dash[i].l;
		}
	}
	lineDash.setPattern(shifted_dash);

	float start = 0;
	float end = reminder;

	lineDash.samplePattern(start, end, sampled_dash);

	for (int i = 0; i < sampled_dash.size(); ++i)
	{
		if (sampled_dash[i].ls == 1)
		{
			float xstart = x1 + sum * ortx * originalPatternLength;
			float ystart = y1 + sum * orty * originalPatternLength;
			xp.push_back(xstart);
			xp.push_back(xstart + sampled_dash[i].l * originalPatternLength * ortx);

			yp.push_back(ystart);
			yp.push_back(ystart + sampled_dash[i].l * originalPatternLength * orty);
		}
		sum += sampled_dash[i].l;
	}
	return reminder;
}

void makeDashedPolyline(
	std::vector<float>& pt_nodes_x,	// (����) ������ x-��������� ������ ����� �������� ���������
	std::vector<float>& pt_nodes_y,	// (����) ������ y-��������� ������ ����� �������� ���������
	std::vector<float> dash, //(����) �������
	float dash_offset, //(����) �������� ���� �������� [-1:+1]
	std::vector<float>& xp,//(�����) ������ x-��������� ��� ��������� ���������
	std::vector<float>& yp,//(�����) ������ y-��������� ��� ��������� ���������
	float & r //(����/�����) �������, ����� ��� ��������� ����������� ��������������� �����.
)
{
	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = 0;
	for (int i = 0; i < pt_nodes_x.size() - 1; i += 1)
	{
		x1 = pt_nodes_x[i];
		y1 = pt_nodes_y[i];
		x2 = pt_nodes_x[i + 1];
		y2 = pt_nodes_y[i + 1];
		r += makeDashedLineSegment(
			x1, y1,
			x2, y2,
			dash, dash_offset - r,
			xp, yp);
	}
}

int main(int argc, char* argv[])
{
	std::vector<float> dash = { 20,6,1,6 };
	float dash_offset = 0.0;
	std::vector<float> xp;
	std::vector<float> yp;
	float r = 0;
	std::vector<float> pt_nodes_x;
	std::vector<float> pt_nodes_y;
	for (int i = 0; i < 100; i += 1)
	{
		pt_nodes_x.push_back (4 * i);
		pt_nodes_y.push_back(100 * sin(pt_nodes_x[i] / 30) + 300);
	}
	
	makeDashedPolyline(pt_nodes_x,pt_nodes_y,dash,dash_offset,xp,yp,r);
	// ���������
	BLImage img(480, 480, BL_FORMAT_PRGB32);
	BLContext ctx(img);
	ctx.setFillStyle(BLRgba32(0xFF555555));
	ctx.setCompOp(BL_COMP_OP_SRC_COPY);
	ctx.fillAll();
	ctx.setStrokeStyle(BLRgba32(0xFFFFffff));
	ctx.setStrokeWidth(2);
	ctx.setFillStyle(BLRgba32(0xFF55ff55));

	for (int i = 0; i < xp.size() / 2; i++)
	{
		ctx.strokeLine(xp[2 * i], yp[2 * i], xp[2 * i + 1], yp[2 * i + 1]);
	}
	ctx.end();

	BLImageCodec codec;
	codec.findByName("BMP");
	img.writeToFile("result.bmp", codec);
	return 0;
}
