// Mar 1, 2021
#include <complex>
#include <valarray>
#include <iostream>
#include <fstream>

static void fast_fourier_transform(std::valarray<std::complex<double>>& array) {
  if (array.size() <= 1) return;
  std::valarray<std::complex<double>> even = array[std::slice(0, array.size() / 2, 2)];
  std::valarray<std::complex<double>> odd  = array[std::slice(1, array.size() / 2, 2)];
  fast_fourier_transform(even);
  fast_fourier_transform(odd);

  for (size_t i = 0, size = array.size(); i < size / 2; ++i) {
    std::complex<double> t = std::polar(1.0, -2 * M_PI * i / size) * odd[i];
    array[i] = even[i] + t;
    array[i + size / 2] = even[i] - t;
  }
}

static void inverse_fast_fourier_transform(std::valarray<std::complex<double>>& array) {
  array = array.apply(std::conj);
  fast_fourier_transform(array);
  array = array.apply(std::conj);
  array /= array.size();
}

void create_graph(const std::string& filename, const std::valarray<std::complex<double>>& data) {
  std::ofstream out_file("/home/machen/" + filename + ".txt");
  for (size_t i = 0; i < data.size(); ++i) {
    out_file << data[i].real() << ' ' << i << std::endl;
  }
  std::string command;
  command += "pushd /home/machen &&";
  command += "gnuplot -e \"filename='" + filename + ".txt';name='" + filename + ".pdf';\" ";
  command += "git/cpp_vk_lib/benchmark/benchmark.gnuplot &&";
  command += "popd &&";
  command += "rm -rf " + filename + ".txt";
  system(command.c_str());
//  system(R"(pushd /home/machen &&
//            gnuplot -e "filename='output.txt';name='output.pdf';" git/cpp_vk_lib/benchmark/benchmark.gnuplot &&
//            popd &&
//            rm -rf output.txt)");
}

void run_fourier_transform_tests() {
  std::valarray<std::complex<double>> data(64);
  for (size_t i = 0; i < data.size(); ++i) {
    data[i] = i;
//    data[i] = std::cos(std::tan(i));
//    data[i] = std::sin(i) * std::pow(i, 1.4);
  }
  create_graph("before_fourier", data);
  fast_fourier_transform(data);
  create_graph("after_fourier", data);
}
