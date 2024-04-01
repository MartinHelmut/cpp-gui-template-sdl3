#pragma once

#include <SDL3/SDL.h>
#include <imgui.h>

#include "Core/Window.hpp"

namespace App {

class DPIHandler {
 public:
  [[nodiscard]] static Window::Settings get_dpi_aware_window_size(const Window::Settings& settings);
};

}  // namespace App
