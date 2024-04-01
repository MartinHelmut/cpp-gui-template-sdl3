#include "Application.hpp"

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>

#include <memory>
#include <string>

#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"
#include "Core/Window.hpp"

namespace App {

Application::Application(const std::string& title) {
  APP_PROFILE_FUNCTION();

  const unsigned int init_flags{SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD};
  if (SDL_Init(init_flags) != 0) {
    APP_ERROR("Error: %s\n", SDL_GetError());
    m_exit_status = ExitStatus::FAILURE;
  }

  m_window = std::make_unique<Window>(Window::Settings{title});
}

Application::~Application() {
  APP_PROFILE_FUNCTION();

  m_window.reset();
  SDL_Quit();
}

ExitStatus App::Application::run() {
  APP_PROFILE_FUNCTION();

  if (m_exit_status == ExitStatus::FAILURE) {
    return m_exit_status;
  }

  m_running = true;
  while (m_running) {
    APP_PROFILE_SCOPE("MainLoop");

    poll_events();
    m_window->update();
  }

  return m_exit_status;
}

void App::Application::stop() {
  APP_PROFILE_FUNCTION();

  m_running = false;
}

void Application::poll_events() {
  APP_PROFILE_FUNCTION();

  SDL_Event event{};
  while (SDL_PollEvent(&event) == 1) {
    APP_PROFILE_SCOPE("EventPolling");

    ImGui_ImplSDL3_ProcessEvent(&event);

    if (event.type == SDL_EVENT_QUIT) {
      stop();
      return;
    }

    if (event.window.windowID == SDL_GetWindowID(m_window->get_native_window())) {
      if (event.window.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
        stop();
        return;
      }

      m_window->on_event(event.window);
    }
  }
}

}  // namespace App
