#pragma once

#include "SDK.h"
#include "CBasePlayer.hpp"
#include <string>
#include <iostream>

#include "CTrace.hpp"

#define __UTL_TOSTRING std::to_string
#define __UTL_PBUF " "

template < class T >
struct UTLCHAR
{
	const char* ptr;
	T val;
};

namespace utils
{
	template < class T >
	std::string GenerateMultiStr(UTLCHAR<T> t)
	{
		std::string temp = "";

		temp.append(t.ptr);
		temp.append(__UTL_PBUF);
		temp.append(__UTL_TOSTRING((T)t.val));

		return temp;
	}

	bool check_scoped(c_baseplayer* player);

	trace_data_t trace_ray(Vector start_pos, Vector end_pos, void* skip_mem);

	bool contains_char(const char* szBase, const char* szToContain);

	Color redGreenGradiant(float percent);

	Color redGreenGradiant(int i, int max);
}