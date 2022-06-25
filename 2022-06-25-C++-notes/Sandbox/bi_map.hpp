// Aug 25, 2021
#ifndef BI_MAP_HPP
#define BI_MAP_HPP

#include <stdexcept>
#include <optional>
#include <map>
#include <any>
#include <set>
#include <unordered_set>
#include <cassert>

#include <iostream>


enum struct bi_map_key : size_t {
    primary,
    secondary
};

template <typename Key1, typename Key2, typename Value>
class bi_map {
public:
    void insert(
        const std::optional<Key1>& key1,
        const std::optional<Key2>& key2,
        const Value& value
    ) {
        if (!key1.has_value() && !key2.has_value()) {
            throw std::invalid_argument("some text");
        }
        if (one_two_keys.count(key1) > 1) {
            throw std::invalid_argument("some text");
        }
        if (parameters.count(key2) > 1) {
            throw std::invalid_argument("some text");
        }
        one_two_keys.emplace(key1, key2);
        parameters.emplace(key2, value);
    }

    template <typename KeyImpl>
    Value& operator()(const KeyImpl& key, enum bi_map_key k) {
        if (k == bi_map_key::primary) {
            return get_by_primary_key(key);
        } else {
            return get_by_secondary_key(key);
        }
    }

    template <typename KeyImpl>
    const Value& operator()(const KeyImpl& key, enum bi_map_key k) const {
        return this->operator()(key, k);
    }

private:
    Value& get_by_primary_key(const Key1& key) {
        auto key1 = one_two_keys.find(key);
        if (key1 == one_two_keys.end()) {
            throw std::out_of_range("some text");
        }
        if (one_two_keys.count(key) > 1) {
            throw std::invalid_argument("some text");
        }
        return parameters.find(one_two_keys.find(key)->second)->second;
    }

    const Value& get_by_primary_key(const Key1& key) const {
        return get_by_primary_key(key);
    }

    Value& get_by_secondary_key(const Key2& key) {
        auto key2 = parameters.find(key);
        if (key2 == parameters.end()) {
            throw std::out_of_range("some text");
        }
        if (parameters.count(key) > 1) {
            throw std::invalid_argument("some text");
        }
        return parameters.find(key)->second;
    }

    const Value& get_by_secondary_key(const Key2& key) const {
        return get_by_secondary_key(key);
    }

    std::map<std::optional<Key1>, std::optional<Key2>> one_two_keys;
    std::map<std::optional<Key2>, Value> parameters;
};

void run_bi_map_tests()
{
    {
        bi_map<std::string, std::string, std::string> map;
        map.insert(std::nullopt, "key2", "value");
        assert(map("key2", bi_map_key::secondary) == "value");
        try {
            map("nullopt", bi_map_key::primary);
            assert(false);
        }  catch (...) { }
    }
    {
        bi_map<std::string, std::string, std::string> map;
        map.insert("key", std::nullopt, "value");
        assert(map("key", bi_map_key::primary) == "value");
        try {
            map("nullopt", bi_map_key::secondary);
            assert(false);
        }  catch (...) { }
    }
}

#endif // BI_MAP_HPP
