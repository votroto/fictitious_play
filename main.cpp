#include <Eigen/Dense>
#include <iostream>
#include <ranges>
#include <vector>

template <typename T>
auto min_idx(T begin, T end)
{
	namespace rgs = std::ranges;

	auto const min_ptr = rgs::min_element(begin, end);
	return rgs::distance(begin, min_ptr);
}

auto fictitious(float *const data, long const width, long const height, size_t const iters)
{
	namespace eig = Eigen;
	using matrixf = eig::Matrix<float, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
	using vectorf = eig::RowVectorXf;

	eig::Map<matrixf> const payoff_c{data, height, width};
	matrixf const payoff_r{payoff_c.transpose()};

	vectorf picked_r{payoff_c.rows()};
	vectorf picked_c{payoff_c.cols()};
	vectorf sum_r{payoff_c.rows()};
	vectorf sum_c{payoff_c.cols()};

	picked_r.setZero();
	picked_c.setZero();
	sum_r.setZero();
	sum_c.setZero();

	for (size_t i = 0; i < iters; i++)
	{
		auto const idx_r = min_idx(std::begin(sum_r), std::end(sum_r));
		picked_r(idx_r) += 1;
		sum_c += payoff_r.row(idx_r);
		auto const idx_c = min_idx(std::begin(sum_c), std::end(sum_c));
		picked_c(idx_c) += 1;
		sum_r -= payoff_c.row(idx_c);
	}

	auto strat_r = picked_r / iters;
	auto strat_c = picked_c / iters;

	std::cout << strat_r << std::endl;
	std::cout << strat_c << std::endl;
}

int main()
{
	std::vector<float> nums;
	long width = 0, height = 0, iters = 100000;
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
