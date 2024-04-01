#include "Window.hpp"

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>

#include <string>

#include "Core/DPIHandler.hpp"
#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"
#include "Core/Resources.hpp"

namespace App {

Window::Window(const Settings& settings)
    : m_settings(DPIHandler::get_dpi_aware_window_size(settings)),
      m_window(SDL_CreateWindow(m_settings.title.c_str(),
          m_settings.width,
          m_settings.height,
          SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY)),
      m_renderer(SDL_CreateRenderer(
          m_window, nullptr, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED)) {
  APP_PROFILE_FUNCTION();

  if (m_renderer == nullptr) {
    APP_ERROR("Error creating SDL_Renderer!");
    return;
  }

  const float scale{SDL_GetWindowDisplayScale(m_window)};
  SDL_SetRenderScale(m_renderer, scale, scale);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io{ImGui::GetIO()};

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable |
                    ImGuiConfigFlags_ViewportsEnable;

  // Absolute imgui.ini path to preserve settings independent of app location.
  static const std::string imgui_ini_filename{m_user_config_path.generic_string() + "imgui.ini"};
  io.IniFilename = imgui_ini_filename.c_str();

  // ImGUI font
  const std::string font_path{Resources::font_path("Manrope.ttf").generic_string()};
  const float font_scaling_factor{SDL_GetWindowDisplayScale(m_window)};
  const float font_size{18.0F * font_scaling_factor};
  io.Fonts->AddFontFromFileTTF(font_path.c_str(), font_size);
  io.FontDefault = io.Fonts->AddFontFromFileTTF(font_path.c_str(), font_size);
  io.FontGlobalScale = 1.0F / font_scaling_factor;

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(m_window, m_renderer);
  ImGui_ImplSDLRenderer3_Init(m_renderer);
}

Window::~Window() {
  APP_PROFILE_FUNCTION();

  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);

  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}

void Window::update() {
  APP_PROFILE_FUNCTION();

  // Start the Dear ImGui frame
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  if (!m_minimized) {
    ImGui::DockSpaceOverViewport();

    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Exit", "Cmd+Q")) {
          on_close();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View")) {
        ImGui::MenuItem("Some Panel", nullptr, &m_show_some_panel);
        ImGui::MenuItem("ImGui Demo Panel", nullptr, &m_show_demo_panel);
        ImGui::MenuItem("Debug Panel", nullptr, &m_show_debug_panel);
        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }

    // Whatever GUI to implement here ...
    if (m_show_some_panel) {
      ImGui::Begin("Some panel", &m_show_some_panel);
      ImGui::Text("Hello World");
      ImGui::End();
    }

    // ImGUI demo panel
    if (m_show_demo_panel) {
      ImGui::ShowDemoWindow(&m_show_demo_panel);
    }

    // Debug panel
    if (m_show_debug_panel) {
      SDL_RendererInfo info;
      SDL_GetRendererInfo(m_renderer, &info);
      const ImGuiIO& io{ImGui::GetIO()};

      ImGui::Begin("Debug panel", &m_show_debug_panel);
      ImGui::Text("Current SDL_Renderer: %s", info.name);
      ImGui::Text("User config path: %s", m_user_config_path.c_str());
      ImGui::Text("Global font scaling %f", io.FontGlobalScale);
      ImGui::End();
    }
  }

  // Rendering
  ImGui::Render();

  SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 255);
  SDL_RenderClear(m_renderer);
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(m_renderer);
}

void Window::on_minimize() {
  APP_PROFILE_FUNCTION();

  m_minimized = true;
}

void Window::on_shown() {
  APP_PROFILE_FUNCTION();

  m_minimized = false;
}

void Window::on_close() {
  APP_PROFILE_FUNCTION();

  SDL_Event window_close_event;
  window_close_event.type = SDL_EVENT_QUIT;
  SDL_PushEvent(&window_close_event);
}

void Window::on_event(const SDL_WindowEvent& event) {
  APP_PROFILE_FUNCTION();

  switch (event.type) {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
      return on_close();
    case SDL_EVENT_WINDOW_MINIMIZED:
      return on_minimize();
    case SDL_EVENT_WINDOW_SHOWN:
      return on_shown();
    default:
      // Do nothing otherwise
      return;
  }
}

SDL_Window* Window::get_native_window() const {
  APP_PROFILE_FUNCTION();

  return m_window;
}

}  // namespace App
