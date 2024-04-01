#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <string>
#include <vector>

#include "Core/Window.hpp"

namespace App {

enum class ExitStatus : std::uint8_t { SUCCESS = 0, FAILURE = 1 };

class Application {
 public:
  explicit Application(const std::string& title);
  ~Application();

  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application other) = delete;
  Application& operator=(Application&& other) = delete;

  ExitStatus run();
  void stop();

 private:
  void poll_events();

  ExitStatus m_exit_status{ExitStatus::SUCCESS};
  std::unique_ptr<Window> m_window{nullptr};

  bool m_running{true};
};

}  // namespace App
