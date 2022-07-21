// Mar 2, 2021
#include <complex>
#include <fstream>
#include <random>
#include <cstdint>


class mandelbrot_generator {
public:
  mandelbrot_generator(
    std::string_view path,
    uint16_t         image_size,
    uint8_t          gamma,
    float            intensity,
    float            scaling,
    float            background_distance
  ) noexcept
    : path_               (path)
    , image_size_         (image_size)
    , gamma_              (gamma)
    , intensity_          (intensity)
    , background_distance_(background_distance)
    , scaling_            (scaling) {}

  void generate() {
    const size_t bytes_to_write = image_size_ * image_size_ * 3;
    std::vector<uint8_t> buffer;
    buffer.resize(bytes_to_write);
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(1, 4);
    const uint8_t r = 1;
    const uint8_t g = 1;
    const uint8_t b = 1;

    for (uint16_t i = 0; i < image_size_; ++i) {
      size_t offset = i * image_size_ * 3;
      for (uint16_t j = 0; j < image_size_; ++j) {
        const uint8_t value = compute_point(i, j);
        buffer[offset++] = static_cast<uint8_t>(value);
        buffer[offset++] = static_cast<uint8_t>(value);
        buffer[offset++] = static_cast<uint8_t>(value);
      }
    }
    std::ofstream png_image(path_.data());
    png_image << "P6\n" << image_size_ << ' ' << image_size_ << ' ' << static_cast<int>(gamma_) << std::endl;
    png_image.write(reinterpret_cast<const char*>(&buffer[0]), bytes_to_write);
  }

private:
  inline __attribute__((always_inline)) uint8_t compute_point(float x, float y) const noexcept {
    std::complex<float> z(0.0f, 0.0f);
    std::complex<float> point(
      x / static_cast<float>(image_size_) - (1.55f / scaling_),
      y / static_cast<float>(image_size_) -   0.5f
    );
    uint8_t iterations = 0;
    while (abs(z) < background_distance_ && iterations++ <= intensity_) {
      z = z * z + point;
      z *= scaling_;
    }
    return iterations < intensity_ ? (255 * iterations) / 40 : 20;
  }

  std::string      path_;
  uint16_t         image_size_;
  uint8_t          gamma_;
  float            intensity_;
  float            background_distance_;
  float            scaling_;
};

void run_gif_writer_tests() {
  int counter = 0;
  for (float scaling = 1.0f; scaling < 3.0f; scaling += 0.03) {
    size_t gamma = 255;
    size_t intensity = 100;
    float background_distance = 2.0f;
    const size_t image_size = 512;
    mandelbrot_generator(
      "/home/krot/images/mandelbrot_" + std::to_string(counter++) + ".png", image_size, gamma, intensity, scaling, background_distance
    ).generate();
  }
  for (float scaling = 3.0f; scaling > 1.0f; scaling -= 0.03) {
    size_t gamma = 255;
    size_t intensity = 100;
    float background_distance = 2.0f;
    const size_t image_size = 512;
    mandelbrot_generator(
      "/home/krot/images/mandelbrot_" + std::to_string(counter++) + ".png", image_size, gamma, intensity, scaling, background_distance
    ).generate();
  }

  std::ofstream gif("/home/krot/images/mandelbrot_animated.gif");
  gif << "GIF89a";
  gif << (512 & 0xFF);
  gif << ((512 >> 8) & 0xFF);
  gif << (512 & 0xFF);
  gif << ((512 >> 8) & 0xFF);
  gif << 0xF0;
  gif << 0;
  gif << 0;

  gif << 0;
  gif << 0;
  gif << 0;

  gif << 0;
  gif << 0;
  gif << 0;
  for (size_t i = 0; i < counter; ++i) {
    std::fstream image("/home/krot/images/mandelbrot_" + std::to_string(i) + ".png", std::ios::app);
    size_t size = image.tellp();
    char* buffer = new char[size];
    image.seekg(0, std::ios::beg);
    image.read(buffer, size);
    gif.write(buffer, size);
    delete[] buffer;
    image.close();
  }

  gif << 0x3b;
}
