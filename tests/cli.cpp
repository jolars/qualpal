#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cstdlib>
#include <memory>
#include <sstream>
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

// Helper to count valid hex colors in output
int
count_hex_colors(const std::string& output)
{
  std::istringstream iss(output);
  std::string line;
  int count = 0;

  while (std::getline(iss, line)) {
    // Trim whitespace
    line.erase(0, line.find_first_not_of(" \t\r\n"));
    line.erase(line.find_last_not_of(" \t\r\n") + 1);

    if (line.length() == 7 && line[0] == '#') {
      // Check if remaining chars are hex
      bool is_hex = true;
      for (size_t i = 1; i < line.length(); ++i) {
        if (!std::isxdigit(line[i])) {
          is_hex = false;
          break;
        }
      }
      if (is_hex)
        count++;
    }
  }
  return count;
}

// Basic functionality tests
TEST_CASE("CLI hex input functionality", "[cli][hex]")
{
  SECTION("basic hex input with multiple colors")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i hex \"#ff0000\" \"#00ff00\" \"#0000ff\"");

    REQUIRE(exit_code == 0);
    REQUIRE_FALSE(output.empty());
    REQUIRE(count_hex_colors(output) == 2);
  }

  SECTION("3-digit hex format")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i hex \"#f00\" \"#0f0\" \"#00f\"");
    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }

  SECTION("6-digit hex format")
  {
    auto [exit_code, output] = run_cli("-n 1 -i hex \"#ff0000\"");
    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 1);
  }

  SECTION("single color generation")
  {
    auto [exit_code, output] = run_cli("-n 1 -i hex \"#ff0000\"");
    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 1);
  }
}

TEST_CASE("CLI palette input functionality", "[cli][palette]")
{
  SECTION("basic palette selection")
  {
    auto [exit_code, output] = run_cli("-n 3 -i palette \"ColorBrewer:Set1\"");

    REQUIRE(exit_code == 0);
    REQUIRE_FALSE(output.empty());
    REQUIRE(count_hex_colors(output) == 3);
  }

  SECTION("request fewer colors than available")
  {
    auto [exit_code, output] = run_cli("-n 2 -i palette \"ColorBrewer:Set1\"");
    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }

  SECTION("request single color from palette")
  {
    auto [exit_code, output] = run_cli("-n 1 -i palette \"ColorBrewer:Set1\"");
    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 1);
  }
}

TEST_CASE("CLI colorspace input functionality", "[cli][colorspace]")
{
  SECTION("basic colorspace generation")
  {
    auto [exit_code, output] =
      run_cli("-n 3 -i colorspace \"0:360\" \"0.5:1\" \"0.3:0.7\"");

    REQUIRE(exit_code == 0);
    REQUIRE_FALSE(output.empty());
    REQUIRE(count_hex_colors(output) == 3);
  }

  SECTION("restricted hue range")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i colorspace \"0:180\" \"0.7:1\" \"0.4:0.6\"");
    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }

  SECTION("narrow saturation range")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i colorspace \"0:360\" \"0.8:1\" \"0.5:0.7\"");
    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }
}

TEST_CASE("CLI CVD simulation functionality", "[cli][cvd]")
{
  SECTION("deutananomaly simulation")
  {
    auto [exit_code, output] =
      run_cli("--deutan 0.5 -n 2 -i hex \"#ff0000\" \"#00ff00\"");

    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }

  SECTION("protanomaly simulation")
  {
    auto [exit_code, output] =
      run_cli("--protan 0.7 -n 2 -i hex \"#ff0000\" \"#00ff00\"");

    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }

  SECTION("tritanomaly simulation")
  {
    auto [exit_code, output] =
      run_cli("--tritan 0.3 -n 2 -i hex \"#ff0000\" \"#0000ff\"");

    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }

  SECTION("multiple CVD simulations combined")
  {
    auto [exit_code, output] = run_cli("--deutan 0.3 --protan 0.2 -n 2 -i hex "
                                       "\"#ff0000\" \"#00ff00\" \"#0000ff\"");

    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }

  SECTION("zero CVD simulation")
  {
    auto [exit_code, output] =
      run_cli("--deutan 0.0 -n 2 -i hex \"#ff0000\" \"#00ff00\"");

    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }

  SECTION("maximum CVD simulation")
  {
    auto [exit_code, output] =
      run_cli("--deutan 1.0 -n 2 -i hex \"#ff0000\" \"#00ff00\"");

    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }
}

TEST_CASE("CLI analyze command functionality", "[cli][analyze]")
{
  SECTION("analyze hex input")
  {
    auto [exit_code, output] =
      run_cli("analyze -i hex \"#ff0000\" \"#00ff00\" \"#0000ff\"");

    REQUIRE(exit_code == 0);
    REQUIRE(output.find("Color Difference Matrix") != std::string::npos);
    REQUIRE(output.find("Colors analyzed: 3") != std::string::npos);
    REQUIRE(output.find("MinDist") != std::string::npos);
  }

  SECTION("analyze palette input")
  {
    auto [exit_code, output] =
      run_cli("analyze -i palette \"ColorBrewer:Set1\"");

    REQUIRE(exit_code == 0);
    REQUIRE(output.find("Color Difference Matrix") != std::string::npos);
    REQUIRE(output.find("Colors analyzed:") != std::string::npos);
  }

  SECTION("analyze with CVD simulation")
  {
    auto [exit_code, output] =
      run_cli("analyze --deutan 0.5 -i hex \"#ff0000\" \"#00ff00\"");

    REQUIRE(exit_code == 0);
    REQUIRE(output.find("Color Difference Matrix") != std::string::npos);
  }

  SECTION("analyze with memory limit")
  {
    auto [exit_code, output] =
      run_cli("analyze --max-memory 0.5 -i hex \"#ff0000\" \"#00ff00\"");

    REQUIRE(exit_code == 0);
    REQUIRE(output.find("Color Difference Matrix") != std::string::npos);
  }
}

TEST_CASE("CLI error handling - input validation", "[cli][error][validation]")
{
  SECTION("invalid hex color format")
  {
    auto [exit_code, output] = run_cli("-n 2 -i hex \"invalid_color\"");

    REQUIRE(exit_code != 0);
    REQUIRE(output.find("Error") != std::string::npos);
  }

  SECTION("non-existent palette")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i palette \"NonExistent:Palette\"");

    REQUIRE(exit_code != 0);
    REQUIRE(output.find("Error") != std::string::npos);
  }

  SECTION("too many colors requested from palette")
  {
    auto [exit_code, output] =
      run_cli("-n 100 -i palette \"ColorBrewer:Set1\"");

    REQUIRE(exit_code != 0);
    REQUIRE(output.find("Error") != std::string::npos);
    REQUIRE(output.find("exceeds palette size") != std::string::npos);
  }

  SECTION("invalid CVD severity values")
  {
    auto [exit_code, output] =
      run_cli("--deutan 1.5 -n 2 -i hex \"#ff0000\" \"#00ff00\"");

    REQUIRE(exit_code != 0);
    REQUIRE(output.find("Error") != std::string::npos);
  }
}

TEST_CASE("CLI error handling - colorspace ranges", "[cli][error][colorspace]")
{
  SECTION("invalid hue range")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i colorspace \"400:500\" \"0:1\" \"0:1\"");
    REQUIRE(exit_code != 0);
  }

  SECTION("invalid saturation range")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i colorspace \"0:360\" \"1.5:2\" \"0:1\"");
    REQUIRE(exit_code != 0);
  }

  SECTION("invalid lightness range")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i colorspace \"0:360\" \"0:1\" \"-0.5:0.5\"");
    REQUIRE(exit_code != 0);
  }
}

TEST_CASE("CLI error handling - argument validation", "[cli][error][arguments]")
{
  SECTION("no values provided")
  {
    auto [exit_code, output] = run_cli("-n 2");

    REQUIRE(exit_code != 0);
    REQUIRE(output.find("Error") != std::string::npos);
  }

  SECTION("wrong number of colorspace arguments - too few")
  {
    auto [exit_code, output] = run_cli("-n 2 -i colorspace \"0:360\" \"0:1\"");
    REQUIRE(exit_code != 0);
  }

  SECTION("wrong number of colorspace arguments - too many")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i colorspace \"0:360\" \"0:1\" \"0:1\" \"extra\"");
    REQUIRE(exit_code != 0);
  }

  SECTION("wrong number of palette arguments")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i palette \"ColorBrewer:Set1\" \"Extra\"");

    REQUIRE(exit_code != 0);
    REQUIRE(output.find("Error") != std::string::npos);
  }
}

TEST_CASE("CLI error handling - analyze command", "[cli][error][analyze]")
{
  SECTION("analyze with no values")
  {
    auto [exit_code, output] = run_cli("analyze");

    REQUIRE(exit_code != 0);
    REQUIRE(output.find("Error") != std::string::npos);
  }

  SECTION("analyze with unsupported colorspace input")
  {
    auto [exit_code, output] =
      run_cli("analyze -i colorspace \"0:360\" \"0:1\" \"0:1\"");

    REQUIRE(exit_code != 0);
    REQUIRE(output.find("Error") != std::string::npos);
    REQUIRE(output.find("not supported for analyze") != std::string::npos);
  }
}

TEST_CASE("CLI background color option", "[cli][background]")
{
  SECTION("background color is accepted and does not error")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i hex \"#ff0000\" \"#00ff00\" -b \"#222222\"");
    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }

  SECTION("invalid background color triggers error")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i hex \"#ff0000\" \"#00ff00\" -b \"notacolor\"");
    REQUIRE(exit_code != 0);
    REQUIRE(output.find("Error") != std::string::npos);
    REQUIRE(output.find("background") != std::string::npos);
  }
}

TEST_CASE("CLI points option for colorspace", "[cli][points]")
{
  SECTION("custom number of points is accepted")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i colorspace \"0:360\" \"0.5:1\" \"0.3:0.7\" -p 50");
    REQUIRE(exit_code == 0);
    REQUIRE(count_hex_colors(output) == 2);
  }
  SECTION("invalid points value triggers error")
  {
    auto [exit_code, output] =
      run_cli("-n 2 -i colorspace \"0:360\" \"0.5:1\" \"0.3:0.7\" -p -10");
    REQUIRE(exit_code != 0);
    REQUIRE(output.find("Error") != std::string::npos);
  }
}
