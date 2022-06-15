#include <utility>
#include <list>
#include <vector>
#include <stdexcept>
#include <cmath>

template <typename Key, typename Value>
class BucketHashmap {
public:
  BucketHashmap()
    : BucketsCount(128)
    , ElementsCount(0)
    , Data(BucketsCount) {}

  /// Θ(Data.size)
  const Value &Find(const Key &K) const {
    auto [Hash, Index] = FindIndex(K);
    if (Index == -1)
      throw std::runtime_error("Element not found in map.");

    return GetValue(Data[Hash][Index]);
  }

  /// Θ(Data.size) + Θ(emplace)) + (Θ(Data.size) if rehash)
  template <typename KeyRef, typename ValueRef>
  void Insert(KeyRef &&K, ValueRef &&V) {
    auto [Hash, Index] = FindIndex(K);
    if (Index != -1) {
      Data[Hash][Index].second = std::forward<ValueRef>(V);
      return;
    }
    Data[Hash].emplace_back(std::forward<KeyRef>(K), std::forward<ValueRef>(V));
    ++ElementsCount;
    if (ElementsCount > BucketsCount * DictCoefficient)
      Rehash(GrowCoefficient * BucketsCount);
  }

  /// Θ(Data.size) + (Θ(Data.size) if rehash)
  void Delete(const Key &K) {
    auto [Hash, Index] = FindIndex(K);
    if (Index == -1)
      return;

    Data[Hash].erase(Data[Hash].begin() + Index);
    --ElementsCount;
    if ((BucketsCount > 1) && (std::pow(GrowCoefficient, 2) *
        ElementsCount < BucketsCount * DictCoefficient))
      Rehash(BucketsCount / GrowCoefficient);
  }

  /// Θ(1)
  std::size_t Size() const {
    return ElementsCount;
  }

private:
  /// Θ(1)
  std::size_t MakeHash(const Key &K) const {
    return std::hash<Key>{}(K) % BucketsCount;
  }

  /// Θ(1)
  const Key &GetKey(const std::pair<Key, Value> &Element) const {
    return Element.first;
  }

  /// Θ(1)
  const Value &GetValue(const std::pair<Key, Value> &Element) const {
    return Element.second;
  }

  /// Θ(Data.size)
  std::pair<std::int64_t, std::int64_t> FindIndex(const Key &K) const {
    const std::size_t Hash{MakeHash(K)};
    for (std::size_t I{0U}; I < Data.at(Hash).size(); ++I) {
      const auto &Element = Data.at(Hash).at(I);
      if (GetKey(Element) == K)
        return {Hash, I};
    }
    return {Hash, -1};
  }

  /// Θ(NewSize)
  void Rehash(std::size_t NewSize) {
    Data.resize(NewSize);
    BucketsCount = NewSize;
  }

  std::size_t BucketsCount;
  std::size_t ElementsCount;
  std::vector<std::vector<std::pair<Key, Value>>> Data;
  static const std::size_t DictCoefficient = 4;
  static const std::size_t GrowCoefficient = 2;
};

#include <cassert>

int main() {
  BucketHashmap<std::string, int> Map;

  for (auto I{0}; I < 25000; ++I)
    Map.Insert(std::to_string(I), I + 1);

  Map.Insert("xxx", 2);
  Map.Insert("yyy", 4);
  assert(Map.Find("xxx") == 2);
  assert(Map.Find("yyy") == 4);
  assert(Map.Find("22322") == 22323);
  assert(Map.Size() == 25002);
  Map.Delete("xxx");
  assert(Map.Size() == 25001);
  try { Map.Find("xxx"); } catch (...) { return 1; }
  assert(false);
}
