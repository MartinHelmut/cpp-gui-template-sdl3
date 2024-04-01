#include "Core/DPIHandler.hpp"

#include "Core/Debug/Instrumentor.hpp"
#include "Core/Window.hpp"

namespace App {

Window::Settings DPIHandler::get_dpi_aware_window_size(const Window::Settings& settings) {
  APP_PROFILE_FUNCTION();

  const SDL_DisplayID primary_display_id{SDL_GetPrimaryDisplay()};
  const float scale{SDL_GetDisplayContentScale(primary_display_id)};
  const int width{static_cast<int>(static_cast<float>(settings.width) * scale)};
  const int height{static_cast<int>(static_cast<float>(settings.height) * scale)};

  return {settings.title, width, height};
}

}  // namespace App
