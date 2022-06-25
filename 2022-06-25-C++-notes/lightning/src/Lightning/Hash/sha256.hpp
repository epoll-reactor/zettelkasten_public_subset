#include <cstring>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "src/Lightning/namespace/lightning.hpp"

class lightning::SHA256 {
public:
    SHA256(const char* msg);
   ~SHA256();
    std::string hash();

protected:
    const char* message;
    int message_size;
    unsigned char* pre_processed_msg;
    int num_tot_bits;
    std::string output;
    void pre_processing();
    void process_512_chunks();
};

const std::string read_from_file(const char* filename);
