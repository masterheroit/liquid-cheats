#pragma once

#ifndef __contentbridge
#define __contentbridge

#include <Windows.h>
#include <string>
#include <urlmon.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>

#pragma comment(lib, "urlmon.lib")

class CContentBridge {
public:
	void run();

	inline bool IsRadarContentOk() {
		return this->m_bRadarContentsLoaded;
	}

	inline bool IsFileExists(const std::string& name) {
		std::ifstream f(name.c_str());
		return f.good();
	}
protected:
	bool m_bRadarContentsLoaded;
};

#endif