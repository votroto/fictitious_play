#include <Eigen/Dense>
#include <iostream>
#include <ranges>
#include <vector>
#include <cxxopts.h>

// Returns the index of the minimum element.
template <typename T>
auto min_idx(T begin, T end)
{
	namespace rgs = std::ranges;

	auto const min_ptr = rgs::min_element(begin, end);
	return rgs::distance(begin, min_ptr);
}

// Implements the fictitious-play algorithm.
auto play(float *const data, long const width, long const height, size_t const iters)
{
	namespace eig = Eigen;
	using matrixf = eig::Matrix<float, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
	using vectorf = eig::RowVectorXf;

	// payoff and payoff.transpose for better locality.
	eig::Map<matrixf> const payoff_c{data, height, width};
	matrixf const payoff_r{payoff_c.transpose()};

	vectorf picked_r{payoff_c.rows()};
	vectorf picked_c{payoff_c.cols()};
	vectorf sub_r{payoff_c.rows()};
	vectorf sum_c{payoff_c.cols()};

	picked_r.setZero();
	picked_c.setZero();
	sub_r.setZero();
	sum_c.setZero();

	// sum_c is summed, sub_r is subtracted. Only need min_idx. Less code
	// -> Smaller icache? (°_°>). idk.
	for (size_t i{0}; i < iters; i++)
	{
		auto const idx_r = min_idx(std::begin(sub_r), std::end(sub_r));
		++picked_r(idx_r);
		sum_c += payoff_r.row(idx_r);
		auto const idx_c = min_idx(std::begin(sum_c), std::end(sum_c));
		++picked_c(idx_c);
		sub_r -= payoff_c.row(idx_c);
	}

	vectorf strat_r = picked_r / iters;
	vectorf strat_c = picked_c / iters;

	return std::make_tuple(strat_r, strat_c);
}

// Reads in the number of iterations, or provides help.
size_t read_opts(int argc, char const *argv[])
{
	auto constexpr exe_name = "fictitious_play";
	auto constexpr exe_desc = "Solves a matrix game using fictitious-play. "
		"Reads a payoff matrix from stdin. Example:\n\n$ cat payoff"
		"\n5 7 4\n3 5 6\n6 4 5\n$ cat payoff | .fictitious_play \n0.25"
		" 0.256 0.494\n0.248 0.256  0.496\n";
	auto const i_type = cxxopts::value<size_t>()->default_value("1000");
	auto constexpr i_name = "i,iterations";
	auto constexpr i_desc = "Number of iterations";
	auto constexpr h_name = "h,help";
	auto constexpr h_desc = "Print help";

	cxxopts::Options options(exe_name, exe_desc);
	options.add_options()(i_name, i_desc, i_type)(h_name, h_desc);

	size_t iters;
	try
	{
		auto result = options.parse(argc, argv);
		if (result.count("help"))
		{
			std::cout << options.help() << std::endl;
			exit(0);
		}
		iters = result["iterations"].as<size_t>();
	}
	catch (const cxxopts::OptionException &e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	return iters;
}

// Reads the payoff matrix from stdin.
auto read_in(std::istream &in)
{
	std::ios_base::sync_with_stdio(false);

	std::vector<float> nums;
	long width{0}, height{0};
	std::string line;
	for (; std::getline(in, line) && !std::empty(line); height++)
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
	return std::make_tuple(nums, width, height);
}

int main(int argc, char const *argv[])
{
	auto iters = read_opts(argc, argv);
	auto [nums, width, height] = read_in(std::cin);

	auto [s_row, s_col] = play(std::data(nums), width, height, iters);

	std::cout << s_row << std::endl;
	std::cout << s_col << std::endl;
}
