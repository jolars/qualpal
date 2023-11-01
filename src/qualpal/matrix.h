#pragma once

#include <array>
#include <vector>

namespace qualpal {

template<typename T>
class Matrix
{
public:
  Matrix(int rows, int cols)
    : rows(rows)
    , cols(cols)
    , data(rows * cols)
  {
  }
  T& operator()(int row, int col) { return data[col * rows + row]; }
  const T& operator()(int row, int col) const { return data[col * rows + row]; }

private:
  int rows, cols;
  std::vector<T> data;
};

template<typename T, int rows, int cols>
class FixedMatrix
{
public:
  FixedMatrix() {}
  FixedMatrix(std::initializer_list<std::initializer_list<T>> list)
  {
    auto it = list.begin();
    for (int i = 0; i < rows; ++i) {
      std::copy(it->begin(), it->end(), data.begin() + i * cols);
      ++it;
    }
  }
  T& operator()(int row, int col) { return data[row * cols + col]; }
  const T& operator()(int row, int col) const { return data[row * cols + col]; }

  std::array<T, rows> operator*(const std::array<T, cols>& vec) const
  {
    std::array<T, rows> result{};
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        result[i] += (*this)(i, j) * vec[j];
      }
    }
    return result;
  }

private:
  std::array<T, rows * cols> data;
};

} // namespace qualpal
