#include "sha256.hpp"

#define ROTR(w, n) (((w) >> (n)) | ((w) << ((sizeof(w) * 8) - (n))))
#define DOS0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ ((x) >> 3))
#define DOS1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ ((x) >> 10))
#define DOSS1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ (ROTR(x, 25)))
#define DOSS0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ((ROTR(x, 22))))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJORITY(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

using lightning::SHA256;

SHA256::SHA256(const char* msg)
    : message(msg)
    , message_size(std::strlen(msg))
{ }

std::string SHA256::hash() {
    pre_processing();
    process_512_chunks();
    delete[] pre_processed_msg;
    return output;
}

void SHA256::pre_processing() {
    unsigned int k, i;
    unsigned int num_bits_to_hash = message_size * 8;
    k = 512 - (64 + 1 + num_bits_to_hash) % 512;
    num_tot_bits = 64 + 1 + num_bits_to_hash + k;
    pre_processed_msg = new unsigned char[message_size];
    memcpy(pre_processed_msg, message, message_size);
    *(pre_processed_msg + (num_bits_to_hash/8)) |= (1 << (7 - (num_bits_to_hash % 8)));
    for (i = 0; i <= (unsigned int) (6 - (num_bits_to_hash % 8)); i++) {
        *(pre_processed_msg + (num_bits_to_hash/8)) &= ~(1 << i);
    }
    for(i = 0; i < 8; i++) {
        *(pre_processed_msg + num_tot_bits/8 - i - 1) = (num_bits_to_hash >> (8*i));
    }
}

void SHA256::process_512_chunks() {
    unsigned int numChunks;
    unsigned int i, j;
    unsigned int w[64];
    uint32_t a, b, c, d, e, f, g, h, T1, T2;
    uint32_t sha_vars[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
    };
    uint32_t constants[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
    unsigned char* chunkPtr, *wcopy;
    numChunks = num_tot_bits/512;
//    printf("number of chunks is %d divided by 512 = %d\n", num_tot_bits, numChunks);
    chunkPtr = (unsigned char*) (pre_processed_msg);
    for (i = 0; i < numChunks; i++) {
        // this only works for little endian machines, it moves chunk into first 16 words and
        // makes sure the bits are in proper positions for little endian
        wcopy = (unsigned char*) w;
        for (j = 0; j < 16; j++) {
            int i = j * 4;
            wcopy[i  ] = chunkPtr[i+3];
            wcopy[i+1] = chunkPtr[i+2];
            wcopy[i+2] = chunkPtr[i+1];
            wcopy[i+3] = chunkPtr[i  ];
        }
        chunkPtr += 64;
        //Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array
        for (j = 16; j < 64; j++) {
            w[j] = w[j-16] + DOS0(w[j-15]) + w[j-7]	+ DOS1(w[j-2]);
        }

        a = sha_vars[0];
        b = sha_vars[1];
        c = sha_vars[2];
        d = sha_vars[3];
        e = sha_vars[4];
        f = sha_vars[5];
        g = sha_vars[6];
        h = sha_vars[7];

        for (j = 0; j < 64; j++) {
            T1 = h + DOSS1(e) + CH(e, f, g) + constants[j] + w[j];
            T2 = DOSS0(a) + MAJORITY(a, b, c);

            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        sha_vars[0] = sha_vars[0] + a;
        sha_vars[1] = sha_vars[1] + b;
        sha_vars[2] = sha_vars[2] + c;
        sha_vars[3] = sha_vars[3] + d;
        sha_vars[4] = sha_vars[4] + e;
        sha_vars[5] = sha_vars[5] + f;
        sha_vars[6] = sha_vars[6] + g;
        sha_vars[7] = sha_vars[7] + h;
  }
    std::stringstream stream;
    for (j = 0; j < 8; j++) {
        stream << std::setfill('0') << std::setw(8) << std::hex << sha_vars[j];
    }
    output = stream.str();
}

const std::string read_from_file(const char* filename) {
    std::ifstream file;
    file.open(filename);
    if (!file) {
        throw std::invalid_argument("file not found");
    } else {
        file.seekg(0, std::ios::end);
        int size = file.tellg();
        file.seekg(0, std::ios::beg);
        char* fcontent = new char[size];
        file.read(fcontent, size);
        file.close();
        return fcontent;
    }
}


SHA256::~SHA256() { }
