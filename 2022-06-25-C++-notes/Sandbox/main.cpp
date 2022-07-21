//#include <iostream>
//#include <fstream>

//#include <boost/asio.hpp>
//#include <boost/asio/spawn.hpp>

//#include <format.hpp>

//int main() {
//  using namespace boost::asio;
//  io_context ctx;
//  std::ifstream in("/home/krot/resources.txt");
//  std::string host_, file_;
//  while (in >> host_ >> file_) {
//    spawn(ctx, [&ctx, host = std::move(host_), file = std::move(file_)](yield_context yield) -> void {
//      boost::system::error_code ec;
//      ip::tcp::resolver resolver(ctx);
//      ip::tcp::resolver::query query(host, "80");
//      auto it = resolver.async_resolve(query, yield[ec]);
//      if (ec || it.empty()) {
//        std::cerr << "Resolve " << host << " failed\n";
//        return;
//      }
//      ip::tcp::socket socket(ctx);
//      socket.async_connect(*it, yield[ec]);
//      if (ec) {
//        std::cerr << "Error async_connect\n";
//        return;
//      }
//      std::string request = format("GET {} HTTP/1.0\r\n\r\n", file);
//      socket.async_send(buffer(request), yield[ec]);
//      if (ec) {
//        std::cerr << "Error async_send";
//        return;
//      }
//      std::vector<char> received;
//      while (true) {
//        std::array<char, 1024> buf;
//        size_t bytes_read = socket.async_read_some(buffer(buf), yield[ec]);
//        if (ec && ec != boost::asio::error::eof) {
//          std::cerr << "Error async_read_some " << ec.message() << std::endl;
//          return;
//        }
//        if (bytes_read == 0)
//          break;
//        received.insert(received.end(), buf.begin(), buf.begin() + bytes_read);
//        std::ofstream outfile(file);
//        for (auto c : buf) {
//          outfile << c;
//        }
//      }
//      std::cout << "Received " << received.size() << " bytes" << std::endl;
//      socket.shutdown(ip::tcp::socket::shutdown_both);
//    });
//  }
//  return ctx.run();
//}




//#include <iostream>

//#include <boost/asio.hpp>
//#include <boost/asio/ssl.hpp>
//#include <boost/bind.hpp>


//using boost::asio::ip::tcp;

//class client {
//public:
//  client(
//    boost::asio::io_service& io_service,
//    boost::asio::ssl::context& ssl_context,
//    const std::string& server,
//    const std::string& path
//  )
//    : resolver(io_service)
//    , socket(io_service, ssl_context)
//  {
//    std::ostream request_stream(&request);
//    request_stream << "GET " << path << " HTTP/1.0\r\n";
//    request_stream << "Host: " << server << "\r\n";
//    request_stream << "Accept: */*\r\n";
//    request_stream << "Connection: close\r\n\r\n";

//    tcp::resolver::query query(server, "https");
//    resolver.async_resolve(query, boost::bind(
//      &client::handle_resolve, this,
//      boost::asio::placeholders::error,
//      boost::asio::placeholders::iterator
//    ));
//  }

//private:
//  void handle_resolve(const boost::system::error_code& ec, tcp::resolver::iterator endpoint_iterator) {
//    if (!ec) {
//      std::cout << "Resolve OK\n";
//      socket.set_verify_mode(boost::asio::ssl::verify_peer);
//      socket.set_verify_callback(boost::bind(
//        &client::verify_certificate, this, _1, _2
//      ));
//      boost::asio::async_connect(socket.lowest_layer(), endpoint_iterator, boost::bind(
//        &client::handle_connect,  this, boost::asio::placeholders::error
//      ));
//    } else {
//      std::cout << "Error resolve: " << ec.message() << std::endl;
//    }
//  }

//  bool verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx) {
//    char subject_name[256];
//    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
//    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
//    std::cout << "Verifying " << subject_name << "\n";
//    return preverified;
//  }

//  void handle_connect(const boost::system::error_code& ec) {
//    if (!ec) {
//      std::cout << "Connect OK\n";
//      socket.async_handshake(boost::asio::ssl::stream_base::client, boost::bind(
//        &client::handle_handshake, this, boost::asio::placeholders::error
//      ));
//    } else {
//      std::cout << "ERROR: " << ec << "\n";
//    }
//  }

//  void handle_handshake(const boost::system::error_code& ec) {
//    if (!ec) {
//      std::cout << "Handshake OK\n";
//      std::cout << "Request: \n";
//      const char* header = boost::asio::buffer_cast<const char*>(request.data());
//      std::cout << header << "\n";
//      boost::asio::async_write(socket, request, boost::bind(
//        &client::handle_write_request, this, boost::asio::placeholders::error
//      ));
//    } else {
//      std::cout << "ERROR: " << ec << "\n";
//    }
//  }

//  void handle_write_request(const boost::system::error_code& ec) {
//    if (!ec) {
//      boost::asio::async_read_until(socket, response, "\r\n", boost::bind(
//        &client::handle_read_status_line, this, boost::asio::placeholders::error
//      ));
//    } else {
//      std::cout << "ERROR: " << ec << "\n";
//    }
//  }

//  void handle_read_status_line(const boost::system::error_code& ec) {
//    if (!ec) {
//      std::istream response_stream(&response);
//      std::string http_version;
//      response_stream >> http_version;
//      uint32_t status_code;
//      response_stream >> status_code;
//      std::string status_message;
//      std::getline(response_stream, status_message);
//      if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
//        std::cout << "Invalid response\n";
//        return;
//      }
//      if (status_code != 200) {
//        std::cout << "Response returned code: " << status_code << "\n";
//        return;
//      }
//      std::cout << "Status code: " << status_code << "\n";
//      boost::asio::async_read_until(socket, response, "\r\n\r\n", boost::bind(
//        &client::handle_read_headers, this, boost::asio::placeholders::error
//      ));
//    } else {
//      std::cout << "ERROR: " << ec << "\n";
//    }
//  }

//  void handle_read_headers(const boost::system::error_code& ec) {
//    if (!ec) {
//      std::istream response_stream(&response);
//      std::string header;
//      while (std::getline(response_stream, header) && header != "\r") {
//        std::cout << header << "\n";
//      }
//      std::cout << "\n";
//      if (response.size() > 0) {
//        std::cout << &response;
//      }
//      boost::asio::async_read(socket, response, boost::asio::transfer_at_least(1), boost::bind(
//        &client::handle_read_content, this, boost::asio::placeholders::error
//      ));
//    } else {
//      std::cout << "ERROR: " << ec << "\n";
//    }
//  }

//  void handle_read_content(const boost::system::error_code& ec) {
//    if (!ec) {
//      std::cout << &response;
//      boost::asio::async_read(socket, response, boost::asio::transfer_at_least(1), boost::bind(
//        &client::handle_read_content, this, boost::asio::placeholders::error
//      ));
//    } else if (ec != boost::asio::error::eof) {
//      std::cout << "ERROR: " << ec << "\n";
//    }
//  }

//  tcp::resolver resolver;
//  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;
//  boost::asio::streambuf request;
//  boost::asio::streambuf response;
//};

//int main(int argc, char** argv) {
//  try {
//    if (argc != 3) {
//      std::cout << "Usage: client <server> <path>\n";
//      return 1;
//    }
//    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
//    ctx.set_default_verify_paths();
//    boost::asio::io_service io_service;
//    client c(io_service, ctx, argv[1], argv[2]);
//    io_service.run();
//  } catch (std::exception& e) {
//    std::cout << "Exception: " << e.what() << std::endl;
//  }
//  return 0;
//}






//#include <memory>
//#include <vector>
//#include <iostream>

//using namespace std;

//class private_base {
//public:
//  void add_child(private_base* ptr) {
//    childs.push_back(ptr);
//  }
//  private_base(private_base* obj_parent = nullptr) {
//    add_child(obj_parent);
//  }
//  virtual ~private_base() {
//    for (auto&& ptr : childs) {
//      delete ptr;
//    }
//  }
//private:
//  vector<private_base*> childs;
//};

//class base_object : public private_base {
//public:
//  base_object(private_base* parent = nullptr) {
//    parent->add_child(this);
//  }
//};

//class application : public private_base {
//public:
//  application(private_base* parent = nullptr)
//    : d1(new base_object(this))
//    , d2(new base_object(this))
//    , d3(new base_object(this))
//  {
//    parent->add_child(this);
//    base_object* alloca = new base_object(this);
//    (void)alloca;
//  }

//private:
//  base_object* d1;
//  base_object* d2;
//  base_object* d3;
//};

//int main() {
//  private_base* parent = new private_base();

//  application* app = new application(parent);
//  application* app2 = new application(parent);

//  (void)app;
//  (void)app2;
//  delete parent;
//}

//#include <iostream>
//#include <iterator>
//#include <vector>
//#include <sstream>
//#include <fstream>

//int main() {
//  std::string input_data;
//  std::getline(std::cin, input_data);
//  std::istringstream in(input_data);

//  std::vector<std::string> data(
//    std::istream_iterator<std::string>{in},
//    std::istream_iterator<std::string>()
//  );
//  std::cout << std::stoi(data[0]) + std::stoi(data[1]) << std::endl;
//}


//#include <iostream>
//#include <cstring>


//struct sha256_buffer {
//  uint64_t  data_size;
//  uint32_t  h[8];
//  uint8_t   last_chunk[64];
//  uint8_t   chunk_size;
//};

//void sha256_init(struct sha256_buffer* buffer);
//void sha256_update(struct sha256_buffer* buffer, const void* data, size_t size);
//void sha256_finalize(struct sha256_buffer* buffer);
//void sha256_read(struct sha256_buffer* buffer, uint8_t* hash);
//void sha256_read_hex(struct sha256_buffer* buffer, char* hex);
//void sha256_easy_hash(const void* data, size_t size, uint8_t* hash);
//void sha256_easy_hash_hex(const void* data, size_t size, char* hex);

//void sha256_init(struct sha256_buffer* buffer) {
//  buffer->h[0] = 0x6a09e667;
//  buffer->h[1] = 0xbb67ae85;
//  buffer->h[2] = 0x3c6ef372;
//  buffer->h[3] = 0xa54ff53a;
//  buffer->h[4] = 0x510e527f;
//  buffer->h[5] = 0x9b05688c;
//  buffer->h[6] = 0x1f83d9ab;
//  buffer->h[7] = 0x5be0cd19;
//  buffer->data_size = 0;
//  buffer->chunk_size = 0;
//}

//static const uint32_t k[64] = {
//  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
//  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
//  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
//  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
//  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
//  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
//  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
//  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
//};

//#define rotate_r(val, bits) (val >> bits | val << (32 - bits))

//static void sha256_calculate_chunk(struct sha256_buffer* buffer, const uint8_t* chunk) {
//  uint32_t w[64];
//  uint32_t tv[8];
//  uint32_t i;

//  for (i = 0; i < 16; ++i) {
//    w[i] =
//      static_cast<uint32_t>(chunk[0]) << 24 |
//      static_cast<uint32_t>(chunk[1]) << 16 |
//      static_cast<uint32_t>(chunk[2]) <<  8 |
//      static_cast<uint32_t>(chunk[3]);
//    chunk += 4;
//  }

//  for (i = 16; i < 64; ++i) {
//    uint32_t s0 = rotate_r(w[i - 15],  7) ^ rotate_r(w[i - 15], 18) ^ (w[i - 15] >>  3);
//    uint32_t s1 = rotate_r(w[i -  2], 17) ^ rotate_r(w[i -  2], 19) ^ (w[i -  2] >> 10);
//    w[i] = w[i - 16] + s0 + w[i - 7] + s1;
//  }

//  for (i = 0; i < 8; ++i) {
//    tv[i] = buffer->h[i];
//  }

//  for (i = 0; i < 64; ++i) {
//    uint32_t s1 = rotate_r(tv[4], 6) ^ rotate_r(tv[4], 11) ^ rotate_r(tv[4], 25);
//    uint32_t ch = (tv[4] & tv[5]) ^ (~tv[4] & tv[6]);
//    uint32_t temp1 = tv[7] + s1 + ch + k[i] + w[i];
//    uint32_t s0 = rotate_r(tv[0], 2) ^ rotate_r(tv[0], 13) ^ rotate_r(tv[0], 22);
//    uint32_t maj = (tv[0] & tv[1]) ^ (tv[0] & tv[2]) ^ (tv[1] & tv[2]);
//    uint32_t temp2 = s0 + maj;

//    tv[7] = tv[6];
//    tv[6] = tv[5];
//    tv[5] = tv[4];
//    tv[4] = tv[3] + temp1;
//    tv[3] = tv[2];
//    tv[2] = tv[1];
//    tv[1] = tv[0];
//    tv[0] = temp1 + temp2;
//  }

//  for (i = 0; i < 8; ++i) {
//    buffer->h[i] += tv[i];
//  }
//}

//void sha256_update(struct sha256_buffer* buffer, const void* data, size_t size) {
//  const uint8_t* ptr = (const uint8_t*)data;
//  buffer->data_size += size;
//  if (size + buffer->chunk_size >= 64) {
//    uint8_t tmp_chunk[64];
//    memcpy(tmp_chunk, buffer->last_chunk, buffer->chunk_size);
//    memcpy(tmp_chunk + buffer->chunk_size, ptr, 64 - buffer->chunk_size);
//    ptr  += (64 - buffer->chunk_size);
//    size -= (64 - buffer->chunk_size);
//    buffer->chunk_size = 0;
//    sha256_calculate_chunk(buffer, tmp_chunk);
//  }
//  while (size >= 64) {
//    sha256_calculate_chunk(buffer, ptr);
//    ptr += 64;
//    size -= 64;
//  }
//  memcpy(buffer->last_chunk + buffer->chunk_size, ptr, size);
//  buffer->chunk_size += size;
//}

//void sha256_finalize(struct sha256_buffer* buffer) {
//  buffer->last_chunk[buffer->chunk_size] = 0x80;
//  buffer->chunk_size++;
//  memset(buffer->last_chunk + buffer->chunk_size, 0, 64 - buffer->chunk_size);

//  if (buffer->chunk_size > 64) {
//    sha256_calculate_chunk(buffer, buffer->last_chunk);
//    memset(buffer->last_chunk, 0, 64);
//  }

//  uint64_t size = buffer->data_size * 8;
//  for (int i = 8; i > 0; --i) {
//    buffer->last_chunk[i + 55] = size & 255;
//    size >>= 8;
//  }
//  sha256_calculate_chunk(buffer, buffer->last_chunk);
//}

//void sha256_read(struct sha256_buffer* buffer, uint8_t* hash) {
//    for (uint32_t i = 0; i < 8; i++) {
//      hash[i * 4    ] = (buffer->h[i] >> 24) & 255;
//      hash[i * 4 + 1] = (buffer->h[i] >> 16) & 255;
//      hash[i * 4 + 2] = (buffer->h[i] >>  8) & 255;
//      hash[i * 4 + 3] =  buffer->h[i]        & 255;
//    }
//}

//static void bin_to_hex(const void* data, uint32_t len, char* out) {
//  static const char* const alphabet = "0123456789abcdef";
//  for (uint32_t i = 0; i < len; ++i) {
//    uint8_t c = ((uint8_t*)data)[i];
//    out[i * 2    ] = alphabet[c >> 4];
//    out[i * 2 + 1] = alphabet[c & 15];
//  }
//}

//void sha256_read_hex(struct sha256_buffer* buffer, char* hex) {
//  uint8_t hash[32];
//  sha256_read(buffer, hash);
//  bin_to_hex(hash, 32, hex);
//}

//void sha256_easy_hash(const void* data, size_t size, uint8_t* hash) {
//  struct sha256_buffer buffer;
//  sha256_init(&buffer);
//  sha256_update(&buffer, data, size);
//  sha256_finalize(&buffer);
//  sha256_read(&buffer, hash);
//}

//void sha256_easy_hash_hex(const void* data, size_t size, char* hex) {
//  uint8_t hash[32];
//  sha256_easy_hash(data, size, hash);
//  bin_to_hex(hash, 32, hex);
//}


//void test_str(const char* in, const char* out) {
//  static int test_count = 0;

//  char buffer[65] = {0};
//  sha256_easy_hash_hex(in, strlen(in), buffer);

//  if (strcmp(out, buffer) != 0) {
//    printf("String test #%i failed!!!\nsha256(\"%s\") = \"%s\"\nExpected value: \"%s\"\n\n", test_count, "{in}", buffer, out);
//    printf("Please report this issue to https://github.com/LekKit/sha256\n");
//    exit(0);
//  } else {
//    printf("String test #%i passed\nsha256(\"%s\") = \"%s\"\n\n", test_count, "{in}", buffer);
//  }

//  test_count++;
//}

//#include <sys/mman.h>
//#include <sys/stat.h>
//#include <fcntl.h>

//int main(int argc, char *argv[]) {
//  struct stat s;
//  int fd = open("/home/krot/TextData/text_files/bluetooth.txt", O_RDONLY);
//  int status = fstat(fd, &s);
//  size_t size = s.st_size;

//  uint8_t* bytes = static_cast<uint8_t*>(mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0));
//  char* text_data = new char[size];
//  for (size_t i = 0; i < size; ++i) {
//    text_data[i] = bytes[i];
//  }

//  test_str(text_data, "8f89484c6b01890bee8a0f1526efaf98827fb0f142c5060f429b1f20c644351b");

//  test_str("", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
//  test_str("abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
//  test_str("haftbefehl", "0c09362cbe3cf528e19c94297a680ad21fa6c83d2a854b2e2a2735362fdc7992");
//  test_str("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1");
//  test_str("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno", "2ff100b36c386c65a1afc462ad53e25479bec9498ed00aa5a04de584bc25301b");
//  test_str("LekKit", "500f94082c97ab2c1188e53d9f467a9e73bfb366e7309adbfac098f0a46d7711");
//  test_str("\xBD", "68325720aabd7c82f30f554b313d0570c95accbb7dc4b5aae11204c08ffe732b");
//  test_str("\xC9\x8C\x8E\x55", "7abc22c0ae5af26ce93dbb94433a0e0b2e119d014f8e7f65bd56c61ccccd9504");

//  delete[] text_data;
//}







//#include <boost/asio.hpp>
//#include <string>
//#include <iostream>

//int main()
//{
//  boost::asio::ip::address_v4 ipa = boost::asio::ip::address_v4::from_string("31.183.145.140");
//  boost::asio::ip::tcp::endpoint ep;
//  ep.address(ipa);

//  boost::asio::io_service service;
//  boost::asio::ip::tcp::resolver resolver(service);
//  boost::asio::ip::tcp::resolver::iterator destination = resolver.resolve(ep);

//  std::cout << destination->host_name() << std::endl;

//  return 0;
//}


//#include <string.h>
//#include <string>
//#include <vector>
//#include <iostream>

//struct Match {
//  Match():_next(0) {}
//  virtual bool match(const char * pattern, const char * input) const {
//    return !strcmp(pattern, input);
//  }
//  bool next(const char * pattern, const char * input) const {
//    if (!_next) return false;
//    return _next->match(pattern, input);
//  }
//  const Match * _next;
//};

//class MatchSet: public Match {
//  typedef std::vector<Match *> Set;
//  Set toTry;
//public:
//  virtual bool match(const char * pattern, const char * input) const {
//    for (Set::const_iterator i = toTry.begin(); i !=toTry.end(); ++i) {
//      if ((*i)->match(pattern, input)) return true;
//    }
//    return false;
//  }
//  void add(Match * m) {
//    toTry.push_back(m);
//    m->_next = this;
//  }
//  ~MatchSet() {
//    for (Set::const_iterator i = toTry.begin(); i !=toTry.end(); ++i)
//      if ((*i)->_next==this) (*i)->_next = 0;
//  }
//};

//struct MatchQuestion: public Match  {
//  virtual bool match(const char * pattern, const char * input) const {
//    if (pattern[0] != '?')
//      return false;
//    if (next(pattern+1, input))
//      return true;
//    if (next(pattern+1, input+1))
//      return true;
//    return false;
//  }
//};

//struct MatchEmpty: public Match {
//  virtual bool match(const char * pattern, const char * input) const {
//    if (pattern[0]==0 && input[0]==0)
//      return true;
//    return false;
//  }
//};

//struct MatchAsterisk: public Match {
//  virtual bool match(const char * pattern, const char * input) const {
//    if (pattern[0] != '*')
//      return false;
//    if (pattern[1] == 0) {
//      return true;
//    }
//    for (int i = 0; input[i] != 0; ++i) {
//      if (next(pattern+1, input+i))
//        return true;
//    }
//    return false;
//  }
//};

//struct MatchPlus: public Match {
//    virtual bool match(const char * pattern, const char * input) const {
//        if (pattern[0] != '+')
//            return false;
//        if (pattern[1] == 0)
//            return true;
//        for (int i = 0; input[i] != 0; ++i) {
//            if (next(pattern + 1, input + i))
//                return true;
//        }
//        return false;
//    }
//};

//struct MatchSymbol: public Match {
//  virtual bool match(const char * pattern, const char * input) const {
//    // TODO: consider cycle here to prevent unnecessary recursion
//    // Cycle should detect special characters and call next on them
//    // Current implementation abstracts from that
//    if (pattern[0] != input[0])
//      return false;
//    return next(pattern+1, input+1);
//  }
//};

//class DefaultMatch: public MatchSet {
//  MatchEmpty empty;
//  MatchQuestion question;
//  MatchAsterisk asterisk;
//  MatchSymbol symbol;
//  MatchPlus plus;
//public:
//  DefaultMatch() {
//    add(&empty);
//    add(&question);
//    add(&asterisk);
//    add(&symbol);
//    add(&plus);
//  }
//  void test(const char * p, const char * input) const {
//    testOneWay(p, input);
////    if (!strcmp(p, input)) return;
////    testOneWay(input, p);
//  }
//  bool testOneWay(const char * p, const char * input) const {
//    const char * eqStr = " == ";
//    bool rv = match(p, input);
//    if (!rv) eqStr = " != ";
//    std::cout << p << eqStr << input << std::endl;
//    return rv;
//  }

//};


//int main(int argc, char* argv[])
//{
//    DefaultMatch d;
//    d.test("a*", "abb");
//    d.test("111?", "111");
//    d.test("t*xt", "text");
//    d.test("t*xt", "teeeeeeext");
//    d.test("t*xt", "txt");
//    d.test("te+xt", "text");
//}


//TreeNode* search(int key, TreeNode* leaf)
//{
//    if (leaf)
//    {
//        if (key == leaf->value)
//        {
//            return leaf;
//        }
//        if (key < leaf->value)
//        {
//            return search(key, leaf->left);
//        }
//        else {
//            return search(key, leaf->right);
//        }
//    }
//    else {
//        return nullptr;
//    }
//}

//TreeNode* search(int key)
//{
//    return search(key, m_root);
//}


//#include <iostream>
//#include <map>

//struct TreeNode
//{
//    int value;
//    TreeNode* left;
//    TreeNode* right;
//};


//class BinaryTree
//{
//public:
//    BinaryTree()
//        : m_root(nullptr)
//    { }

//    ~BinaryTree()
//    {
//        destroy_tree();
//    }

//    void insert(int key)
//    {
//        if (m_root)
//        {
//            m_value_counts[key]++;
//            insert(key, m_root);
//        }
//        else {
//            m_root = new TreeNode;
//            m_root->value = key;
//            m_root->left = nullptr;
//            m_root->right = nullptr;

//            m_value_counts[key]++;
//        }
//    }

//    void destroy_tree()
//    {
//        destroy_tree(m_root);
//    }

//    void print_in_ascending_order()
//    {
//        print_in_order(m_root);
//    }


//    void print_with_count()
//    {
//        for (auto&& element : m_value_counts)
//        {
//            std::cout << element.first << " " << element.second << std::endl;
//        }
//    }

////    bool check_if_avl()
////    {
////        return check_if_avl(m_root);
////    }


//private:
//    int max_depth(TreeNode* leaf)
//    {
//        if (!leaf)
//        {
//            return 0;
//        }
//        else {
//            int l_depth = max_depth(leaf->left);
//            int r_depth = max_depth(leaf->right);

//            if (l_depth > r_depth)
//            {
//                return l_depth + 1;
//            }
//            else {
//                return r_depth + 1;
//            }
//        }
//    }

////    bool check_if_avl(TreeNode* leaf)
////    {
////        int left;
////        int right;

////        if (!leaf)
////        {
////            return 1;
////        }

////        left = max_depth(leaf->left);
////        right = max_depth(leaf->right);

////        if (abs(left - right) <= 1 && check_if_avl(leaf->left) && check_if_avl(leaf->right))
////        {
////            return true;
////        }

////        return false;
////    }

//    void print_in_order(TreeNode* leaf)
//    {
//        if (!leaf)
//        {
//            return;
//        }

//        print_in_order(leaf->left);

//        std::cout << leaf->value << std::endl;

//        print_in_order(leaf->right);
//    }

//    void destroy_tree(TreeNode* leaf)
//    {
//        if (leaf)
//        {
//            destroy_tree(leaf->left);
//            destroy_tree(leaf->right);
//            delete leaf;
//        }
//    }

//    void insert(int key, TreeNode* leaf)
//    {
//        if (key < leaf->value)
//        {
//            if (leaf->left)
//            {
//                insert(key, leaf->left);
//            }
//            else {
//                leaf->left = new TreeNode;
//                leaf->left->value = key;
//                leaf->left->left = nullptr;
//                leaf->left->right = nullptr;
//            }
//        }
//        else if (key >= leaf->value)
//        {
//            if (leaf->right)
//            {
//                insert(key, leaf->right);
//            }
//            else {
//                leaf->right = new TreeNode;
//                leaf->right->value = key;
//                leaf->right->left = nullptr;
//                leaf->right->right = nullptr;
//            }
//        }
//    }

//    TreeNode* m_root;
//    std::map<int, int> m_value_counts;
//};

//int main()
//{
//    BinaryTree* tree = new BinaryTree;

//    while (true)
//    {
//        int digit;
//        std::cin >> digit;

//        if (digit == 0)
//        {
//            break;
//        }

//        tree->insert(digit);
//    }

//    tree->print_with_count();

//    delete tree;
//    return 0;
//}











//#define ELEMS 1000000
//#define REPS 50

//#include <iostream>
//#include <cassert>
//#include <vector>

//#include "memory_pool.hpp"

//int main()
//{
//  clock_t start;

//  std::cout << "Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com\n";
//  std::cout << "Provided to compare the default allocator to MemoryPool.\n\n";

//  /* Use the default allocator */
//  StackAllocator<int, std::allocator<int> > stackDefault;
//  start = clock();
//  for (int j = 0; j < REPS; j++)
//  {
//    assert(stackDefault.empty());
//    for (int i = 0; i < ELEMS / 4; i++) {
//      // Unroll to time the actual code and not the loop
//      stackDefault.push(i);
//      stackDefault.push(i);
//      stackDefault.push(i);
//      stackDefault.push(i);
//    }
//    for (int i = 0; i < ELEMS / 4; i++) {
//      // Unroll to time the actual code and not the loop
//      stackDefault.pop();
//      stackDefault.pop();
//      stackDefault.pop();
//      stackDefault.pop();
//    }
//  }
//  std::cout << "Default Allocator Time: ";
//  std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

//  /* Use MemoryPool */
//  StackAllocator<int, MemoryPool<int> > stackPool;
//  start = clock();
//  for (int j = 0; j < REPS; j++)
//  {
//    assert(stackPool.empty());
//    for (int i = 0; i < ELEMS / 4; i++) {
//      // Unroll to time the actual code and not the loop
//      stackPool.push(i);
//      stackPool.push(i);
//      stackPool.push(i);
//      stackPool.push(i);
//    }
//    for (int i = 0; i < ELEMS / 4; i++) {
//      // Unroll to time the actual code and not the loop
//      stackPool.pop();
//      stackPool.pop();
//      stackPool.pop();
//      stackPool.pop();
//    }
//  }
//  std::cout << "MemoryPool Allocator Time: ";
//  std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";


//  std::cout << "Here is a secret: the best way of implementing a stack"
//            " is a dynamic array.\n";

//  /* Compare MemoryPool to std::vector */
//  std::vector<int> stackVector;
//  start = clock();
//  for (int j = 0; j < REPS; j++)
//  {
//    assert(stackVector.empty());
//    for (int i = 0; i < ELEMS / 4; i++) {
//      // Unroll to time the actual code and not the loop
//      stackVector.push_back(i);
//      stackVector.push_back(i);
//      stackVector.push_back(i);
//      stackVector.push_back(i);
//    }
//    for (int i = 0; i < ELEMS / 4; i++) {
//      // Unroll to time the actual code and not the loop
//      stackVector.pop_back();
//      stackVector.pop_back();
//      stackVector.pop_back();
//      stackVector.pop_back();
//    }
//  }
//  std::cout << "Vector Time: ";
//  std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

//  std::cout << "The vector implementation will probably be faster.\n\n";
//  std::cout << "MemoryPool still has a lot of uses though. Any type of tree"
//            " and when you have multiple linked lists are some examples (they"
//            " can all share the same memory pool).\n";

//  return 0;
//}



//#include <vector>
//#include <unordered_map>
//#include <queue>
//#include <iostream>


//class GraphEdge;

//class GraphNode
//{
//public:
//    GraphNode(int node_index)
//        : m_node_index(node_index)
//    { }

//    std::vector<GraphEdge*> get_edges() const noexcept
//    {
//        return m_edges;
//    }

//    void set_edges(const std::vector<GraphEdge*>& edges)
//    {
//        m_edges = edges;
//    }

//    int get_node_index() const noexcept
//    {
//        return m_node_index;
//    }

//private:
//    std::vector<GraphEdge*> m_edges;
//    int m_node_index;
//};


//class GraphEdge
//{
//public:
//    GraphEdge(int weight, GraphNode* node1, GraphNode* node2)
//        : m_weight(weight)
//        , m_node1(node1)
//        , m_node2(node2)
//    { }

//    int weight() const noexcept
//    {
//        return m_weight;
//    }

//    GraphNode* get_node1() const noexcept
//    {
//        return m_node1;
//    }

//    GraphNode* get_node2() const noexcept
//    {
//        return m_node2;
//    }

//    bool operator==(GraphEdge* edge)
//    {
//        if (edge->m_node1 == m_node1)
//        {
//            if (edge->m_node2 == m_node2)
//                return true;
//        }
//        else if (edge->m_node1 == m_node2)
//        {
//            if (edge->m_node2 == m_node1)
//                return true;
//        }
//        else if (edge->m_node2 == m_node2)
//        {
//            if (edge->m_node1 == m_node1)
//                return true;
//        }
//        else if (edge->m_node2 == m_node1)
//        {
//            if (edge->m_node1 == m_node2)
//                return true;
//        }

//        return false;
//    }

//private:
//    int m_weight;
//    GraphNode* m_node1;
//    GraphNode* m_node2;
//};


//using adjacency_list_t = std::unordered_map<GraphNode*, std::vector<GraphEdge*>>;
//using adjacency_matrix_t = std::vector<std::vector<int>>;

//class Graph
//{
//public:
//    Graph(int num_verticies, adjacency_matrix_t&& adjacency_matrix)
//        : m_num_verticies(num_verticies)
//        , m_adjacency_matrix(std::move(adjacency_matrix))
//    {
//        create_nodes();
//        create_edges();
//    }

//    virtual ~Graph()
//    {
//        for (auto&& item : m_verticies)
//            delete item;

//        for (auto&& item : m_edges)
//            delete item;
//    }

//    GraphNode* add_node(int index)
//    {
//        auto* node = new GraphNode(index);
//        m_verticies.push_back(node);

//        auto* edges = new std::vector<GraphEdge*>;
//        node->set_edges(*edges);
//        return node;
//    }

//    GraphEdge add_edge(GraphNode* node_1, GraphNode* node_2, int weight)
//    {
//        auto* edge = new GraphEdge(weight, node_1, node_2);

//        m_edges.push_back(edge);
//        node_1->get_edges().push_back(edge);

//        return *edge;
//    }

//    void create_nodes()
//    {
//        for (int i = 0; i < m_num_verticies; ++i)
//            add_node(i);
//    }

//    void create_edges()
//    {
//        for (int i = 0; i < m_num_verticies; ++i)
//        {
//            for (int j = 0; j < m_num_verticies; ++j)
//            {
//                if (m_adjacency_matrix[i][j] != 0)
//                    add_edge(m_verticies.at(i), m_verticies.at(j), m_adjacency_matrix[i][j]);
//            }
//        }
//    }

//    const std::vector<GraphNode*> get_verticies() const
//    {
//        return m_verticies;
//    }
//    std::vector<GraphEdge>* prim_algorithm(GraphNode* node)
//    {
//        return nullptr;
//    }
//    std::vector<GraphEdge>* dijkstra_algorithm(GraphNode* node);

//private:
//    void print_parents(int index, int source_index, int* parent_verticies)
//    {
//        if (index != source_index)
//        {
//            print_parents(parent_verticies[index], source_index, parent_verticies);
//            std::cout << " -> " << std::endl;
//        }
//        std::cout << index;
//    }

//    struct Comparator
//    {
//        int operator()(GraphEdge& edge_1, GraphEdge& edge_2)
//        {
//            return edge_1.weight() > edge_2.weight();
//        }
//    };

//    int m_num_verticies;
//    adjacency_matrix_t m_adjacency_matrix;
//    std::vector<GraphEdge*> m_edges;
//    std::vector<GraphNode*> m_verticies;
//};

//int main()
//{
//    int num_verticies;
//    std::cin >> num_verticies;
//    std::vector<std::vector<int>> matrix(num_verticies);

//    for (int i = 0; i < num_verticies; ++i)
//    {
//        for (int j = 0; j < num_verticies; ++j)
//        {
//            int val;
//            std::cin >> val;
//            matrix.at(i).push_back(val);
//        }
//    }

//    Graph graph(num_verticies, std::move(matrix));
//    std::vector<GraphEdge>* edges_1 = graph.prim_algorithm(graph.get_verticies().at(0));

//    int sum = 0;

//    for (auto&& item : *edges_1)
//    {
//        std::cout << item.get_node1()->get_node_index() << ", "
//                  << item.get_node2()->get_node_index() << ", "
//                  << item.weight();
//        std::cout << std::endl;

//        sum += item.weight();
//    }


//}


#include <bits/stdc++.h>

#include "testSpeed.hpp"


std::map<std::string, std::string> f(std::map<std::string, std::string> map)
{
    map["213"] = "123";
    return map;
}

int main()
{
    std::string dummy;

    testFuncSpeed(25000, [&dummy](){
        std::map<std::string, std::string> params = {
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},        {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},        {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            };
        params = f(params);
        dummy += params["100000000000000000000000000000000"];
    });

    testFuncSpeed(25000, [&dummy](){
        std::map<std::string, std::string> params = {
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},        {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},        {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            {"1000000000000000000000000000000000", "20000000000000000000000000000000000000000000000000000000000000000000000000000"},
            };
        params = f(std::move(params));
        dummy += params["1000000000000000000000000000000000"];
    });

    std::cout << dummy[2];

    return 0;
}















