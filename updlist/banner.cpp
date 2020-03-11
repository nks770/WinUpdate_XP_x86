
#include "stdafx.h"
#include "banner.h"
#include <string>

void banner(const bool batch_mode, const char* display_date, std::string ProductName) {
	if(batch_mode) {
		printf("echo  __      ___       _   _           _       _\n");
		printf("echo  \\ \\    / (_)_ __ ^| ^| ^| ^|_ __   __^| ^| __ _^| ^|_ ___\n");
		printf("echo   \\ \\/\\/ /^| ^| '_ \\^| ^| ^| ^| '_ \\ / _` ^|/ _` ^| __/ _ \\\n");
		printf("echo    \\    / ^| ^| ^| ^| ^| ^|_^| ^| ^|_) ^| (_^| ^| (_^| ^| ^|^|  __/\n");
		printf("echo     \\/\\/  ^|_^|_^| ^|_^|\\___/^| .__/ \\__,_^|\\__,_^|\\__\\___^|\n");
		printf("echo                         ^|_^|     Updated %s\n",display_date);
	} else {
		printf(" __      ___       _   _           _       _\n");
		printf(" \\ \\    / (_)_ __ | | | |_ __   __| | __ _| |_ ___\n");
		printf("  \\ \\/\\/ /| | '_ \\| | | | '_ \\ / _` |/ _` | __/ _ \\\n");
		printf("   \\    / | | | | | |_| | |_) | (_| | (_| | ||  __/\n");
		printf("    \\/\\/  |_|_| |_|\\___/| .__/ \\__,_|\\__,_|\\__\\___|\n");
		printf("                        |_|     Updated %s\n",display_date);
	}

	printf("%s\n",batch_mode?"echo.":"");
	printf("%s          %s\n",batch_mode?"echo ":"",ProductName.c_str());
	printf("%s\n",batch_mode?"echo.":"");
}