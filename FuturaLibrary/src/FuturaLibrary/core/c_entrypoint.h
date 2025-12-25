#pragma once

#ifdef FT_PLATFORM_WINDOWS

#include <memory>
#include "c_application.h"

int main(int argc, char** argv)
{
	FuturaLibrary::EventLog::Init();

	auto app = FuturaLibrary::CreateApplication("assets/");
	app->Run();
	return 0;
}

#endif 