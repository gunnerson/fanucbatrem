#include "bg.hpp"
#include "buttons.hpp"
#include "hazard.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <ctime>
#include <windows.h>

bool isOnButton(int x, int y);

int main() {

  auto now = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(now);
  struct tm tmBuf;
  localtime_s(&tmBuf, &t);
  wchar_t today[16];
  wcsftime(today, sizeof(today) / sizeof(today[0]), L"%Y-%m", &tmBuf);

  HKEY hKey;
  LONG result;
  const wchar_t *subKey = L"SOFTWARE\\FanucBatRem";
  const wchar_t *valueName = L"Last";
  wchar_t lastValue[32] = {0};
  DWORD lastValueSize = sizeof(lastValue);
  DWORD dwType = REG_SZ;

  result = RegCreateKeyExW(HKEY_CURRENT_USER, subKey, 0, NULL,
                           REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL,
                           &hKey, NULL);

  if (result == ERROR_SUCCESS) {
    result =
        RegQueryValueExW(hKey, valueName, NULL, &dwType,
                         reinterpret_cast<LPBYTE>(lastValue), &lastValueSize);

    if (result != ERROR_SUCCESS || dwType != REG_SZ) {
      const wchar_t defaultValue[] = L"2024-01";
      RegSetValueExW(
          hKey, valueName, 0, REG_SZ,
          reinterpret_cast<const BYTE *>(defaultValue),
          static_cast<DWORD>((wcslen(defaultValue) + 1) * sizeof(wchar_t)));
    }

    RegCloseKey(hKey);

  } else {
    return EXIT_FAILURE;
  }

  if (lastValue[0] != L'\0') {
    int lastYear{0}, lastMonth{0};
    if (swscanf(lastValue, L"%4d-%2d", &lastYear, &lastMonth) == 2) {

      if (lastYear - 1900 >= tmBuf.tm_year ||
          ((tmBuf.tm_year + 1900 - lastYear == 1) &&
           (tmBuf.tm_mon < lastMonth))) {
        return EXIT_SUCCESS;
      }
    }
  }

  // sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  // unsigned int width = desktop.size.x;
  // unsigned int height = desktop.size.y;

  auto window{sf::RenderWindow(
      sf::VideoMode({980, 720}), "Fanuc Battery Reminder",
      sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed)};
  window.setFramerateLimit(30);

  const auto cursor_arrow =
      sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow);
  const auto cursor_hand = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand);

  auto position = sf::Mouse::getPosition(window);

  const sf::Texture bg_tx{bg_png, bg_png_len};
  sf::Sprite bg_s(bg_tx);
  sf::RectangleShape bg_overlay({980, 720});

  const sf::Texture buttons_tx{buttons_png, buttons_png_len};
  sf::Sprite button1(buttons_tx, sf::IntRect({0, 0}, {120, 120}));
  button1.setPosition({430, 580});
  sf::Sprite button2(buttons_tx, sf::IntRect({121, 0}, {120, 120}));
  button2.setPosition({430, 580});

  const sf::Texture hazard_tx{hazard_png, hazard_png_len};
  sf::Sprite hazard_s(hazard_tx);
  hazard_s.setPosition({670, 10});

  sf::Font font;
  if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
    return EXIT_FAILURE;
  }

  sf::Text text1(font);
  text1.setString("REPLACE");
  text1.setCharacterSize(128);
  text1.setFillColor(sf::Color::Red);
  text1.setStyle(sf::Text::Bold);
  text1.setPosition({40, 80});

  sf::Text text2(font);
  text2.setString("BATTERIES!");
  text2.setCharacterSize(128);
  text2.setFillColor(sf::Color::Red);
  text2.setStyle(sf::Text::Bold);
  text2.setPosition({40, 260});

  sf::Text text3(font);
  text3.setString(
      "Low battery charge will cause the loss of machine calibration!");
  text3.setCharacterSize(28);
  text3.setFillColor(sf::Color::Red);
  text3.setStyle(sf::Text::Bold);
  text3.setPosition({60, 440});

  sf::Text text4(font);
  text4.setString("REPLACED");
  text4.setCharacterSize(64);
  text4.setFillColor(sf::Color::White);
  text4.setStyle(sf::Text::Bold);
  text4.setPosition({320, 500});

  sf::Text text5(font);
  text5.setString("+1 847-888-3498");
  text5.setCharacterSize(24);
  text5.setFillColor(sf::Color(65, 123, 116));
  text5.setStyle(sf::Text::Bold);
  text5.setPosition({20, 680});

  sf::Text text6(font);
  text6.setString("service@schneeberger-us.com");
  text6.setCharacterSize(24);
  text6.setFillColor(sf::Color(65, 123, 116));
  text6.setStyle(sf::Text::Bold);
  text6.setPosition({600, 680});

  const auto onClose{[&](const sf::Event::Closed &) { window.close(); }};

  const auto onKeyPressed{[&](const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scancode::Escape) {
      window.close();
    }
  }};

  const auto onMouseClicked{
      [&](const sf::Event::MouseButtonReleased &mouseButtonReleased) {
        if (mouseButtonReleased.button == sf::Mouse::Button::Left) {

          if (window.hasFocus() && isOnButton(position.x, position.y)) {

            result = RegCreateKeyExW(HKEY_CURRENT_USER, subKey, 0, NULL,
                                     REG_OPTION_NON_VOLATILE,
                                     KEY_READ | KEY_WRITE, NULL, &hKey, NULL);

            if (result == ERROR_SUCCESS) {
              RegSetValueExW(
                  hKey, valueName, 0, REG_SZ,
                  reinterpret_cast<const BYTE *>(today),
                  static_cast<DWORD>((wcslen(today) + 1) * sizeof(wchar_t)));

              RegCloseKey(hKey);
              window.close();
            }
          }
        }
      }};

  int i{255}, d{-10};
  while (window.isOpen()) {

    if (i <= 55) {
      d = 10;
    } else if (i >= 255) {
      d = -10;
    }
    hazard_s.setColor(sf::Color(255, 255, 255, i += d));

    position = sf::Mouse::getPosition(window);
    window.handleEvents(onClose, onKeyPressed, onMouseClicked);

    window.clear(sf::Color::Black);
    window.draw(bg_s);
    window.draw(hazard_s);
    window.draw(text1);
    window.draw(text2);
    window.draw(text3);
    window.draw(text5);
    window.draw(text6);

    if (window.hasFocus() && isOnButton(position.x, position.y)) {
      window.draw(button2);
      window.setMouseCursor(cursor_hand.value());
      window.draw(text4);
    } else {
      window.draw(button1);
      window.setMouseCursor(cursor_arrow.value());
    }

    window.display();
  }
}

bool isOnButton(int x, int y) {
  return (490 - x) * (490 - x) + (640 - y) * (640 - y) <= 52 * 52;
}
