#include "qualpal/colors.h"
#include "qualpal/qualpal.h"
#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::List
qualpal_cpp(int n,
            const std::vector<double> r,
            const std::vector<double> g,
            const std::vector<double> b)
{
  int N = r.size();

  std::vector<qualpal::RGB> rgb_colors;

  for (int i = 0; i < N; ++i) {
    rgb_colors.emplace_back(r[i], g[i], b[i]);
  }

  std::vector<qualpal::RGB> selected_colors = qualpal::qualpal(n, rgb_colors);

  std::vector<double> r_out;
  std::vector<double> g_out;
  std::vector<double> b_out;

  std::vector<double> h_out;
  std::vector<double> s_out;
  std::vector<double> l_out;

  std::vector<double> l99d_out;
  std::vector<double> a99d_out;
  std::vector<double> b99d_out;

  std::vector<std::string> hex_out;

  for (const auto& rgb : selected_colors) {
    qualpal::DIN99d din99d = rgb;
    qualpal::HSL hsl = rgb;
    std::string hex = rgb.hex();

    r_out.emplace_back(rgb.r());
    g_out.emplace_back(rgb.g());
    b_out.emplace_back(rgb.b());

    h_out.emplace_back(hsl.h());
    s_out.emplace_back(hsl.s());
    l_out.emplace_back(hsl.l());

    l99d_out.emplace_back(din99d.l());
    a99d_out.emplace_back(din99d.a());
    b99d_out.emplace_back(din99d.b());

    hex_out.emplace_back(hex);
  }

  return Rcpp::List::create(Rcpp::Named("r") = r_out,
                            Rcpp::Named("g") = g_out,
                            Rcpp::Named("b") = b_out,
                            Rcpp::Named("h") = h_out,
                            Rcpp::Named("s") = s_out,
                            Rcpp::Named("l") = l_out,
                            Rcpp::Named("l99d") = l99d_out,
                            Rcpp::Named("a99d") = a99d_out,
                            Rcpp::Named("b99d") = b99d_out,
                            Rcpp::Named("hex") = hex_out);
}
