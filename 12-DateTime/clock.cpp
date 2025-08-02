#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

#include <atomic>
#include <chrono>
#include <cmath>
#include <format>
#include <functional>
#include <future>
#include <iostream>
#include <locale>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../shared/mygetopt.h"

using namespace ftxui;

constexpr int defaultSize = 64;
constexpr int defaultMargin = 8;
constexpr int defaultPadding = 4;

int clock_size = defaultSize;
int clock_margin = defaultMargin;
int clock_padding = defaultPadding;

void print_usage(const char* argv0) {
    std::cerr << "Usage\n\n  " << argv0 << " [options]\n\n";
    std::cerr << "Displays an analog clock in the terminal.\n\n";
    std::cerr << "Options\n\n";
    std::cerr << "  -h | --help           = Print this help\n" \
                 "  -s | --size number    = Size of clock\n" \
                 "  -m | --margin number  = Spacing between margin and clock\n" \
                 "  -p | --padding number = Spacing between clock and hour marks\n\n";
    std::cerr << std::endl;
}

int parse_arguments(int argc, char* argv[]) {

    static const option longOptions[] = {
        { "help", 0, nullptr, 'h'},
        { "size", 1, nullptr, 's' },
        { "margin", 1, nullptr, 'm' },
        { "padding", 1, nullptr, 'p' },
        { nullptr, 0, nullptr, 0 },
    };

    int opt = -1;

    while ((opt = getopt_long(argc, argv, "hs:m:p:", longOptions, nullptr)) > 0) {
        switch (opt) {
            case 'h':
                print_usage(argv[0]);
                exit(0);
            case 's':
                clock_size = atoi(optarg);
                if (clock_size < 0) clock_size = defaultSize;
                break;
            case 'm':
                clock_margin = atoi(optarg);
                if (clock_margin < 0) clock_margin = defaultMargin;
                break;
            case 'p':
                clock_padding = atoi(optarg);
                if (clock_padding < 0) clock_padding = defaultPadding;
                break;
            case '?':
                print_usage(argv[0]);
                exit(1);
        }
    }

    return optind;
}

template<float divisions>
void draw_clockhand(float value, Color color, Canvas& canvas, int w, int h, float length = 1.0f) {
    float angle = value * (2.0 * M_PI / divisions) - 0.5 * M_PI;
    int x = static_cast<int>((w >> 1) + ((w >> 1) - clock_margin) * cos(angle) * length);
    int y = static_cast<int>((h >> 1) + ((h >> 1) - clock_margin) * sin(angle) * length);
    canvas.DrawPointLine(w >> 1, h >> 1, x, y, color);
}

void show_clock(ScreenInteractive& screen, int radius) {
  auto clock = Renderer([&] {
    auto w = (radius << 1) - 5;
    auto h = (radius << 1) - 9;
    auto c = Canvas(radius << 1, radius << 1);

    c.DrawPointLine(0, 0, w, 0);
    c.DrawPointLine(w, 0, w, h);
    c.DrawPointLine(w, h, 0, h);
    c.DrawPointLine(0, h, 0, 0);

    // A circle using braille characters
    c.DrawPointCircle(w >> 1, h >> 1, (w >> 1) - clock_margin);

    // Date complication
    const std::chrono::zoned_time<std::chrono::system_clock::duration> local_now{ std::chrono::current_zone(), std::chrono::system_clock::now() };
    const std::chrono::year_month_day local_date{ std::chrono::floor<std::chrono::days>(local_now.get_local_time()) };
    const auto date{ std::format("{:%a, %d. %B}", local_date) };

    c.DrawText((w >> 1) - date.size(),
               (h >> 1) + (h / 6),
               date,
               Color::White);

    // Hour marks
    for (int i = 1; i <= 12; ++i) {
      const double angle = i * (M_PI / 6) - 0.5 * M_PI;
      const int x = static_cast<int>((w >> 1) + ((w >> 1) - clock_margin - clock_padding) * cos(angle));
      const int y = static_cast<int>((h >> 1) + ((h >> 1) - clock_margin - clock_padding) * sin(angle));

      Pixel p;
      p.character = std::format("{:d}", i);
      c.DrawPixel(x, y, p);
    }

    // Clock hands
    const auto hh_mm_ss = std::chrono::hh_mm_ss<std::chrono::system_clock::duration>(local_now.get_local_time().time_since_epoch());
    const auto sec = static_cast<float>(hh_mm_ss.seconds().count() % 60);
    const auto min = static_cast<float>(hh_mm_ss.minutes().count() % 60);
    const auto hrs = static_cast<float>(hh_mm_ss.hours().count() % 12);

    draw_clockhand<12.0f>(hrs + min / 60.0, Color::White, c, w, h, 0.5);
    draw_clockhand<60.0f>(min + sec / 60.0, Color::White, c, w, h, 0.9);
    draw_clockhand<60.0f>(sec, Color::Red, c, w, h);

    return canvas(std::move(c));
  });

  clock |= CatchEvent([&](Event e) {
    if (e == Event::Escape) {
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });

  screen.Loop(clock);
}

std::atomic_bool stop_update{ false };

void update_clock(ScreenInteractive& screen) {
  while (!stop_update) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(500ms);
    screen.RequestAnimationFrame();
  }
}

int main(int argc, char* argv[]) {

  auto index = parse_arguments(argc, argv);
  if (argc - optind != 0) {
      print_usage(argv[0]);
      return 1;
  }

  // Initialize screen
  auto screen = ScreenInteractive::FitComponent();
  screen.TrackMouse(false);

  // Launch periodic update thread & draw clock
  auto result = std::async(std::launch::async, update_clock, std::ref(screen));
  show_clock(screen, clock_size);

  // Stop thread and clean up
  stop_update = true;
  screen.Clear();

  return 0;
}
