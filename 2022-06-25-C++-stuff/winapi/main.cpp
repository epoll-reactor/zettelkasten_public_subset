#include <wine/windows/windows.h>
#include <wine/windows/iphlpapi.h>

#include <string>
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <memory>

#pragma comment(lib, "iphlpapi.lib")

class mac_utility {
public:
  explicit mac_utility()
    : adapter(new IP_ADAPTER_INFO) { }

  ~mac_utility() {
    delete adapter;
  }

  std::string get_mac_address() {
    if (GetAdaptersInfo(adapter, &buffer_length) == ERROR_BUFFER_OVERFLOW) {
      delete adapter;
      adapter = new IP_ADAPTER_INFO;
    }

    if (GetAdaptersInfo(adapter, &buffer_length) != NO_ERROR) {
      return {};
    }

    return format_mac(adapter);
  }

private:
  static const std::size_t mac_octets = 6;
  DWORD buffer_length = sizeof(IP_ADAPTER_INFO);
  PIP_ADAPTER_INFO adapter;

  std::string format_mac(PIP_ADAPTER_INFO adapter_info) {
    std::ostringstream ss;
    std::string mac_addr;
    do {
      for (std::size_t i = 0; i < mac_octets; i++) {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)(adapter_info->Address[i]);
        if (i < 5) {
          ss << ':';
        }
      }

      mac_addr = ss.str();
      ss.clear();
      ss.str("");

      adapter_info = adapter_info->Next;
    } while(adapter_info);
    return mac_addr;
  }
};

class hdd_utility {
public:
DWORD get_serial_number() {
  DWORD VolumeSerialNumber = 0;
  GetVolumeInformation("c:\\", NULL, NULL, &VolumeSerialNumber, NULL, NULL, NULL, NULL);
  return VolumeSerialNumber;
}
};


int main() {
  mac_utility mac_util;
  hdd_utility hdd_util;
  std::cout << mac_util.get_mac_address() << std::endl;
  std::cout << hdd_util.get_serial_number() << std::endl;
}

