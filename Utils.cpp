#include "Utils.hpp"
#include "SDK.h"

bool utils::check_scoped(c_baseplayer* player)
{
	if (!player || !(player->Alive())) return false;

	if (!player || !player->get_weapon()) return false;

	return true;
}

trace_data_t utils::trace_ray(Vector start_pos, Vector end_pos, void* skip_mem)
{
	CGameTrace trace;
	CTraceFilter filter;

	Ray_t ray;

	filter.pSkip = skip_mem;

	ray.Init(start_pos, end_pos);

	I::Trace->TraceRay(ray, MASK_SHOT, &filter, &trace);

	return trace_data_t{
		trace.fraction,
		trace.fractionleftsolid,
		trace.startpos,
		trace.endpos
	};
}

bool utils::contains_char(const char* szBase, const char* szToContain)
{
	std::string str = "";
	str.append(szBase);

	return str.find(szToContain) != std::string::npos;
}

Color utils::redGreenGradiant(float percent)
	{
		if (percent < 0) {
			return Color(255, 0, 0, 255);
		}

		int r = 0, g = 0, b = 0;
		if (percent > 1) {
			g = 255;
			b = (int)(255 * (percent - 1) / 0.5);
		}
		else if (percent < 0.5) {
			r = 255;
			g = (int)(255 * percent / 0.5); // closer to 0.5, closer to yellow (255,255,0)
		}
		else {
			g = 255;
			r = 255 - (int)(255 * (percent - 0.5) / 0.5); // closer to 1.0, closer to green (0,255,0)
		}
		return Color(r, g, b, 255);
	}

Color utils::redGreenGradiant(int i, int max) {
	float percent = float(i) / float(max);
	return redGreenGradiant(percent);
}