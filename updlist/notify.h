
#ifndef NOTIFY_H
#define NOTIFY_H

#include "stdafx.h"
#include <vector>
#include <string>

void servicePackNotifications(int sp, std::vector<std::string> *data);
void showNotifications(bool batch, std::vector<std::string> *data);

#endif