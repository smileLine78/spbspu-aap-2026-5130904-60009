#include <cstddef>
#include <fstream>
#include <iostream>
#include <new>

namespace shulzhenko
{
  constexpr std::size_t max_size = 10000;

  bool readSize(std::istream &input, std::size_t &rows, std::size_t &cols)
  {
    long long int rows_val = 0;
    long long int cols_val = 0;

    if (!(input >> rows_val >> cols_val))
    {
      return false;
    }

    if (rows_val < 0 || cols_val < 0)
    {
      return false;
    }

    rows = static_cast<std::size_t>(rows_val);
    cols = static_cast<std::size_t>(cols_val);
    return true;
  }

  bool readElements(std::istream &input, int *matrix, std::size_t size)
  {
    for (std::size_t i = 0; i < size; ++i)
    {
      if (!(input >> matrix[i]))
      {
        return false;
      }
    }
    return true;
  }

  int notSameElements(const int *matrix, std::size_t rows, std::size_t cols)
  {
    int count = 0;

    for (std::size_t i = 0; i < rows; ++i)
    {
      bool ok = true;

      for (std::size_t j = 0; j + 1 < cols; ++j)
      {
        if (matrix[i * cols + j] == matrix[i * cols + j + 1])
        {
          ok = false;
          break;
        }
      }

      if (ok)
      {
        ++count;
      }
    }

    return count;
  }

  int minSum(const int *matrix, std::size_t rows, std::size_t cols)
  {
    if (rows == 0 || cols == 0)
    {
      return 0;
    }

    const std::size_t d_count = rows + cols - 1;
    bool first = true;
    int min_sum = 0;

    for (std::size_t d = 0; d < d_count; ++d)
    {
      int sum = 0;

      for (std::size_t i = 0; i < rows; ++i)
      {
        if (i <= d)
        {
          const std::size_t j = d - i;

          if (j < cols)
          {
            sum += matrix[i * cols + j];
          }
        }
      }

      if (first || sum < min_sum)
      {
        min_sum = sum;
        first = false;
      }
    }

    return min_sum;
  }

  bool run(std::istream &input, std::ostream &output, char mode)
  {
    std::size_t rows = 0;
    std::size_t cols = 0;

    if (!readSize(input, rows, cols))
    {
      return false;
    }

    const std::size_t total = rows * cols;
    long long int result1 = 0;
    long long int result2 = 0;

    if (mode == '1')
    {
      if (total > max_size)
      {
        return false;
      }

      int matrix[max_size];

      if (!readElements(input, matrix, total))
      {
        return false;
      }

      result1 = notSameElements(matrix, rows, cols);
      result2 = minSum(matrix, rows, cols);
    }
    else
    {
      int *matrix = nullptr;

      if (total > 0)
      {
        matrix = new int[total];

        if (matrix == nullptr)
        {
          return false;
        }

        if (!readElements(input, matrix, total))
        {
          delete[] matrix;
          return false;
        }
      }

      result1 = notSameElements(matrix, rows, cols);
      result2 = minSum(matrix, rows, cols);
      delete[] matrix;
    }

    output << result1 << '\n';
    output << result2 << '\n';

    return output.good();
  }
}

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    return 1;
  }

  const char *const mode = argv[1];
  const char *const input_path = argv[2];
  const char *const output_path = argv[3];

  if ((mode[0] != '1' && mode[0] != '2') || mode[1] != '\0')
  {
    return 1;
  }

  std::ifstream input{input_path};

  if (!input)
  {
    return 1;
  }

  std::ofstream output{output_path};

  if (!output)
  {
    return 1;
  }

  if (!shulzhenko::run(input, output, mode[0]))
  {
    return 2;
  }

  return 0;
}
