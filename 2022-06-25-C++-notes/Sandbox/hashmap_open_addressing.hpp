// Apr 11, 2022
#include <iostream>
#include <vector>
#include <limits>

class OpenAddressingHashMap {
  struct Entry {
    explicit Entry(int Key_ = NotPresent) : Key(Key_) {}
    int Key;
  };

public:
  OpenAddressingHashMap(int Size) : TableSize(Size), Table(TableSize) {}

  /// Θ(1 - TableSize)
  void Add(int Key) {
    std::size_t Index = LinearProbe(Key, /*IsSearching=*/false);
    Table[Index].Key = Key;
    if (++CurrentSize / static_cast<double>(TableSize) >= 0.5) /// Load factor.
      Rehash();
  }

  void addInfo(int Key) {
    std::cout << "Initial table: ";
    display();
    std::cout << std::endl;
    std::cout << "hash of " << Key << " is " << MakeHash(Key) << " % "
              << TableSize << " = " << MakeHash(Key) % TableSize;
    std::cout << std::endl;
    Add(Key);
    std::cout << "New table: ";
    display();
  }

  /// Θ(1 - TableSize)
  void Remove(int Key) {
    std::size_t Index = LinearProbe(Key, /*IsSearching=*/true);
    if (Index == NotPresent) {
      std::cout << "key not found" << std::endl;
      return;
    }
    std::cout << "Removal Successful, leaving tomb" << std::endl;
    Table[Index].Key = Tomb;
    CurrentSize--;
  }

  void RemovalInfo(int key) {
    std::cout << "Initial table: ";
    display();
    std::cout << std::endl;
    std::cout << "hash of " << key << " is " << MakeHash(key) << " % "
              << TableSize << " == " << MakeHash(key) % TableSize;
    std::cout << std::endl;
    Remove(key);
    std::cout << "New table: ";
    display();
  }

  /// Θ(1 - TableSize)
  const Entry &Find(int Key) {
    const Entry &entry = Table[LinearProbe(Key, /*IsSearching=*/true)];
    if (entry.Key == NotPresent) {
      std::cout << "Key not present";
    }
    return entry;
  }

  void display() {
    for (int i = 0; i < TableSize; i++) {
      if (Table[i].Key == NotPresent) {
        std::cout << " Empty ";
      } else if (Table[i].Key == Tomb) {
        std::cout << " Tomb ";
      } else {
        std::cout << " ";
        std::cout << Table[i].Key;
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }

  /// Θ(1)
  size_t MakeHash(int Key) {
    return std::hash<int>{}(Key);
  }

private:
  /// Θ(1)
  bool PutProber(const Entry& E) {
    return (E.Key == NotPresent || E.Key == Tomb);
  }

  /// Θ(1)
  bool SearchingProber(const Entry& E, int key) {
    return E.Key == key;
  }

  /// Θ(1 - TableSize)
  int LinearProbe(int Key, bool IsSearching) {
    std::size_t KeyHash = MakeHash(Key);
    std::size_t I = 0;
    Entry Entry;
    do {
      int Index = (KeyHash + I) % TableSize;
      Entry = Table[Index];
      if (IsSearching) {
        if (Entry.Key == NotPresent)
          return NotPresent;
        if (SearchingProber(Entry, Key)) {
          std::cout << "Found key!\n";
          return Index;
        }
        std::cout << "Found tombstone or equal hash, checking next\n";
        I++;
      } else {
        if (PutProber(Entry)) {
          if (!NeedsRehash)
            std::cout << "Spot found!\n";
          return Index;
        }
        if (!NeedsRehash)
          std::cout << "Spot taken, looking at next\n";
        I++;
      }
      if (TableSize == I) {
        std::cout << "Linear probe failed\n";
        return NotPresent;
      }
    } while (Entry.Key != NotPresent);
    return NotPresent;
  }

  /// Θ(TableSize * 2)
  void Rehash() {
    NeedsRehash = true;
    int OldSize = TableSize;
    std::vector<Entry> OldTable = Table;
    TableSize *= 2;
    Table = std::vector<Entry>(TableSize);
    for (std::size_t I = 0U; I < OldSize; I++) {
      if (OldTable[I].Key != NotPresent) {
        CurrentSize--;
        Add(OldTable[I].Key);
      }
    }
    NeedsRehash = false;
    std::cout << "Table was rehashed, new size is: " << TableSize << std::endl;
  }

public:
  static const std::size_t NotPresent = std::numeric_limits<std::size_t>::min();

private:
  int TableSize{};
  std::vector<Entry> Table{};
  int Tomb = -1;
  int CurrentSize{};
  bool NeedsRehash{};
};

void RunOpenAddressingHashMapTests() {
  int cmd = 0, key = 0;
  std::cout << "Enter the initial size of Hash Table: ";
  int totalSize;
  std::cin >> totalSize;
  OpenAddressingHashMap map(totalSize);
  bool loop = true;
  while (loop) {
    std::cout << std::endl;
    std::cout << "Options -" << std::endl;
    std::cout << "  1. Add key. (Numeric only)" << std::endl;
    std::cout << "  2. Remove key." << std::endl;
    std::cout << "  3. Find key." << std::endl;
    std::cout << "  4. Generate Hash. (Numeric only)" << std::endl;
    std::cout << "  5. Display Hash table." << std::endl;
    std::cout << "  6. Exit." << std::endl;
    std::cin >> cmd;
    switch (cmd) {
      case 1:
        std::cout << "Enter key to Add = ";
        std::cin >> key;
        map.addInfo(key);
        break;
      case 2:
        std::cout << "Enter key to remove = ";
        std::cin >> key;
        map.RemovalInfo(key);
        break;
      case 3: {
        std::cout << "Enter key to search = ";
        std::cin >> key;
        if (const auto &entry = map.Find(key); entry.Key == OpenAddressingHashMap::NotPresent) {
          std::cout << "Key not present";
        }
        break;
      }
      case 4:
        std::cout << "Enter element to generate hash = ";
        std::cin >> key;
        std::cout << "Hash of " << key
                  << " is = " << map.MakeHash(key);
        break;
      case 5:
        map.display();
        break;
      default:
        loop = false;
        break;
    }
    std::cout << std::endl;
  }
}
