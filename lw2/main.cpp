#include "BMP.h"
#include <chrono>
#include <fstream>
#include <thread>
#include <vector>

using namespace std::chrono;

struct Args
{
	std::string inputSrc;
	std::string outputSrc;
	int threadCount;
	int radius;
};

struct Pixel
{
	int b, g, r;
};

double Square(double num)
{
	return num * num;
}

Args ParseArgs(int argc, char* argv[])
{
	if (argc != 5)
	{
		throw std::invalid_argument("Please pass valid params: <input.bmp> <output.bmp> <threadsCount> <radius>");
	}

	int threadCount;
	int radius;
	try
	{
		threadCount = std::stoi(argv[3]);
		radius = std::stoi(argv[4]);
	}
	catch (const std::logic_error& e)
	{
		throw std::invalid_argument("Threads count and radius should be valid integers");
	}

	if (threadCount <= 0)
	{
		throw std::invalid_argument("Threads count and radius should be positive numbers");
	}

	return { argv[1], argv[2], threadCount, radius };
}

int GetIndex(int x, int y, int w)
{
	return 3 * (x + w * y);
}

int GetAvgColor(int x, int y, int w, int shift, std::vector<uint8_t>::const_iterator begin, int radius)
{
	int totalColor = 0;
	for (int dx = -radius; dx < radius; dx++)
	{
		for (int dy = -radius; dy < radius; dy++)
		{
			totalColor += begin[GetIndex(x + dx, y + dy, w) + shift];
		}
	}

	return totalColor / Square(2 * radius + 1);
}

Pixel BlurPixel(int x, int y, int w, std::vector<uint8_t>::const_iterator begin, int radius)
{
	return {
		GetAvgColor(x, y, w, 0, begin, radius),
		GetAvgColor(x, y, w, 1, begin, radius),
		GetAvgColor(x, y, w, 2, begin, radius)
	};
}

void BlurRect(const BMP& src, BMP& dst, int x1, int x2, int radius)
{
	int imgWidth = src.bmp_info_header.width;
	int imgHeight = src.bmp_info_header.height;

	for (int y = radius; y < imgHeight - radius; y++)
	{
		for (int x = x1; x < x2; x++)
		{
			auto [b, g, r] = BlurPixel(x, y, imgWidth, src.data.begin(), radius);
			dst.set_pixel(x, y, b, g, r, 0);
		}
	}
}

std::vector<std::pair<int, int>> SplitForRects(int w, int threadCount, int radius)
{
	std::vector<std::pair<int, int>> bounds;
	int lineW = (w - radius) / threadCount;

	int x = radius;

	for (int i = 0; i < threadCount; i++)
	{
		bounds.emplace_back(x, x + lineW);
	}

	auto [left, _] = bounds.back();
	bounds.back() = { left, w - radius};

	return bounds;
};

int main(int argc, char* argv[])
{
	std::string inputSrc, outputSrc;
	int threadCount;
	int blurRadius;
	try
	{
		auto [in, out, threads, radius] = ParseArgs(argc, argv);
		inputSrc = in;
		outputSrc = out;
		threadCount = threads;
		blurRadius = radius;
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what() << '\n';
	}

	BMP src(inputSrc.c_str());
	BMP dst(inputSrc.c_str());

	int w = src.bmp_info_header.width;

	auto rectBounds = SplitForRects(w, threadCount, blurRadius);

	const auto start = steady_clock::now();
	{
		std::vector<std::jthread> threads;
		for (int k = 0; k < threadCount; k++)
		{
			auto [x1, x2] = rectBounds[k];
			threads.emplace_back(BlurRect, std::cref(src), std::ref(dst), x1, x2, blurRadius);
		}
	}
	const auto finish = steady_clock::now();
	auto duration = duration_cast<milliseconds>(finish - start).count();

	dst.write(outputSrc.c_str());
	std::cout << std::format("{} {} {} {}", threadCount, blurRadius, duration, std::thread::hardware_concurrency());
}