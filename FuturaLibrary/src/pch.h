#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <thread>

#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <regex>

#ifdef FT_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#ifdef defined(FT_PLATFORM_LINUX)
	#include <X11/Xlib.h>
#endif