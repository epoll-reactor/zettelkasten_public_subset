/*
 * Given an array of integers nums and an integer target, return
 * indices of the two numbers such that they add up to target.
 *
 * You may assume that each input would have exactly one solution, and you may not use the same element twice.
 * You can return the answer in any order.
 *
 * Input: nums = [2,7,11,15], target = 9
 * Output: [0,1]
 * Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].
 */

#include <unordered_map>
#include <vector>
#include <iostream>

std::vector<int> two_sum(std::vector<int> &&numbers, int target)
{
	std::unordered_map<int, int> indicies;
	indicies.reserve(numbers.size() / 4);

	for (size_t i{0U}; i < numbers.size(); ++i)
	{
		int sum{target - numbers.at(i)};
		if (indicies.find(sum) != indicies.end())
		{
			return { indicies[sum], static_cast<int>(i) };
		}
		indicies.emplace(numbers.at(i), i);
	}
	
	return { 0, 0 };
}

int main()
{
	for (auto i : two_sum({2, 7, 11, 15}, 9)) std::cout << i << std::endl;
}
