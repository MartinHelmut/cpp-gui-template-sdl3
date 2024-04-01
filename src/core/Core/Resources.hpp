#pragma once

#include <SDL3/SDL.h>

#include <filesystem>
#include <string_view>

#include "Settings/Project.hpp"

namespace App {

class Resources {
 public:
  [[nodiscard]] static std::filesystem::path get_user_config_path() {
    return SDL_GetPrefPath(COMPANY_NAMESPACE.c_str(), APP_NAME.c_str());
  }

  [[nodiscard]] static std::filesystem::path resource_path(const std::filesystem::path& file_path);
  [[nodiscard]] static std::filesystem::path font_path(const std::string_view& font_file);
};

}  // namespace App
