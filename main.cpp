#include <Eigen/Dense>
#include <iostream>
#include <ranges>
#include <vector>

namespace eig = Eigen;
namespace rgs = std::ranges;

template <typename T>
auto min_idx(T begin, T end)
{
	auto const min_ptr = rgs::min_element(begin, end);
	return rgs::distance(begin, min_ptr);
}

auto fictitious(float *const data, long const width, long const height, size_t const iters)
{
	eig::Map<eig::MatrixXf> const payoff_c{data, height, width};
	eig::MatrixXf const payoff_r{payoff_c.transpose()};

	eig::VectorXf picked_r{payoff_r.rows()};
	eig::VectorXf picked_c{payoff_r.cols()};
	eig::VectorXf sum_r{payoff_r.rows()};
	eig::VectorXf sum_c{payoff_r.cols()};

	picked_r.setZero();
	picked_c.setZero();
	sum_r.setZero();
	sum_c.setZero();

	for (size_t i = 0; i < iters; i++)
	{
		auto const idx_r = min_idx(std::begin(sum_r), std::end(sum_r));
		picked_r(idx_r) += 1;
		sum_c += payoff_c.col(idx_r);
		auto const idx_c = min_idx(std::begin(sum_c), std::end(sum_c));
		picked_c(idx_c) += 1;
		sum_r -= payoff_r.col(idx_c);
	}

	auto strat_r = picked_r /iters;
	auto strat_c = picked_c /iters;

	
	auto xx = std::make_tuple(strat_r, strat_c);
	auto [stra_r, stra_c] = xx;
	std::cout << eig::Transpose(stra_r) << std::endl;
	std::cout << eig::Transpose(stra_c) << std::endl;
}

int main()
{
	std::vector<float> nums;
	long width = 0, height = 0, iters = 100;
	std::string line;
	std::ios_base::sync_with_stdio(false);
	for (; std::getline(std::cin, line) && !std::empty(line); height++)
	{
		long current_width = 0;
		float num;
		std::istringstream line_stream{line};
		for (; line_stream >> num; current_width++)
		{
			nums.emplace_back(num);
		}
		if (width == 0)
		{
			width = current_width;
		}
		if (width != current_width)
		{
			std::cerr << "Error: ragged array." << std::endl;
			exit(1);
		}
	}

	fictitious(std::data(nums), width, height, iters);
}
