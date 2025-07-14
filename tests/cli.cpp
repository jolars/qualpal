#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cstdlib>
#include <memory>
#include <string>

// Helper to run CLI and capture output
std::pair<int, std::string>
run_cli(const std::string& args)
{
  std::string command = "./qualpal " + args + " 2>&1"; // Capture stderr too
  std::array<char, 128> buffer;
  std::string result;

  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"),
                                                pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }

  int exit_code = pclose(pipe.release()) / 256; // Get exit code
  return { exit_code, result };
}

TEST_CASE("CLI basic hex input", "[cli]")
{
  auto [exit_code, output] =
    run_cli("-n 2 -i hex \"#ff0000\" \"#00ff00\" \"#0000ff\"");

  REQUIRE(exit_code == 0);
  REQUIRE_FALSE(output.empty());

  // Count lines of output
  size_t line_count = std::count(output.begin(), output.end(), '\n');
  REQUIRE(line_count >= 2); // Should have at least 2 colors
}

TEST_CASE("CLI error handling", "[cli]")
{
  auto [exit_code, output] = run_cli("-n 2 -i hex \"invalid_color\"");

  REQUIRE(exit_code != 0); // Should fail
  REQUIRE(output.find("Error") != std::string::npos);
}
