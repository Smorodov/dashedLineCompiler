
#include <blend2d.h>
#include <dash.h>

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
	// отрисовка
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
	codec.findByName("PNG");
	img.writeToFile("result.png", codec);
	return 0;
}
