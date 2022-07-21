// Mar 1, 2022
#include "AudioFile.hpp"
#include "fourier_transform.hpp"

struct valarray_cast {
  valarray_cast(const std::vector<double>& array)
    : array_(array) {}

  operator std::valarray<std::complex<double>>() const {
    std::valarray<std::complex<double>> range(array_.size());
    for (size_t i = 0; i < range.size(); ++i) {
      range[i] = array_[i];
    }
    return range;
  }

  const std::vector<double>& array_;
};

void wav_process_tests() {
  std::string filename = "/home/krot/record.wav";
  AudioFile<double> audio_file;
  bool loaded_ok = audio_file.load(filename);
  assert(loaded_ok);

  printf("channels   : %d\n", audio_file.getNumChannels());
  printf("framerate  : %d\n", audio_file.getNumSamplesPerChannel());
  printf("bit depth  : %d\n", audio_file.getBitDepth());
  printf("length     : %f\n", audio_file.getLengthInSeconds());
  printf("sample rate: %d\n", audio_file.getSampleRate());

  for (int i = 0; i < audio_file.getNumChannels(); ++i) {
    auto& channel = audio_file.samples[i];

    std::valarray<std::complex<double>> fft_data = valarray_cast{channel};
//    create_graph("before", range);
    fast_fourier_transform(fft_data);
    inverse_fast_fourier_transform(fft_data);
    create_graph("after", fft_data);

//    for (size_t j = range.size() / 2; j > 0; --j) {
//      channel[j] = range[j].real();
//      channel[range.size() - j - 1] = range[j].real();
//    }

    for (size_t j = 0; j < fft_data.size(); ++j) {
      channel[j] = fft_data[j].real();
    }
  }
  audio_file.save("/home/krot/processed-record.wav");
}
