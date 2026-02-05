#pragma once
#include <core/game.hpp>
#include <imgui.h>

#include <core/world.hpp>
#include <entities/entity.hpp>

#include <fstream>

std::string saveFileDialog();
std::string openFileDialog();

bool endsWith(const std::string& s, const std::string& suffix);
