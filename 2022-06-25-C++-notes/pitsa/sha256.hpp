#define watch(x)	\
	std::cout		\
	<< (#x)			\
	<< " is "		\
	<< "("			\
	<< (x)			\
	<< ")"			\
	<< std::endl

class SHA256 {
public:
  SHA256();
	void update(const char* data);
	static void to_string(char result[65], const unsigned char* digest);
	unsigned char* digest();
	void states_reset();
	static char* read_from_file(const char* filename);

private:
	static const unsigned int SHA256_constants[64];
	unsigned char data[64];
	static unsigned int blocklen;
	static unsigned long long bitlen;
	static unsigned int states[8];
	inline void sha256gen();
	inline void pad();
  inline void revert(unsigned char* hash);
};
