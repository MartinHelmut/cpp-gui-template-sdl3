#pragma once

#include <SDL3/SDL.h>

#include <filesystem>
#include <string>

#include "Core/Resources.hpp"

namespace App {

class Window {
 public:
  struct Settings {
    std::string title;
    int width{1280};
    int height{720};
  };

  explicit Window(const Settings& settings);
  ~Window();

  Window(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window other) = delete;
  Window& operator=(Window&& other) = delete;

  void update();
  void on_minimize();
  void on_shown();
  static void on_close();
  void on_event(const SDL_WindowEvent& event);

  [[nodiscard]] SDL_Window* get_native_window() const;

 private:
  const Settings m_settings;
  SDL_Window* m_window;
  SDL_Renderer* m_renderer;

  bool m_minimized{false};

  const std::filesystem::path m_user_config_path{Resources::get_user_config_path()};

  bool m_show_some_panel{true};
  bool m_show_debug_panel{false};
  bool m_show_demo_panel{false};
};

}  // namespace App
