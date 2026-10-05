#include "BMP.h"
#include <chrono>
#include <fstream>
#include <thread>
#include <vector>

struct Args
{
	std::string inputSrc;
	std::string outputSrc;
	int threadCount;
};

struct Pixel
{
	int b, g, r;
};

Args ParseArgs(int argc, char* argv[])
{
	if (argc != 4)
	{
		throw std::invalid_argument("");
	}

	int threadCount;
	try
	{
		threadCount = std::stoi(argv[3]);
	}
	catch (const std::logic_error& e)
	{
		throw std::invalid_argument("Threads count should be valid integer");
	}

	if (threadCount <= 0)
	{
		throw std::invalid_argument("Threads count should be positive number");
	}

	return { argv[1], argv[2], threadCount };
}

int GetIndex(int x, int y, int w)
{
	return 3 * (x + w * y);
}

int GetAvgColor(int x, int y, int w, int shift, std::vector<uint8_t>::const_iterator begin)
{
	int totalColor = begin[GetIndex(x - 1, y - 1, w) + shift]
		+ begin[GetIndex(x, y - 1, w) + shift]
		+ begin[GetIndex(x + 1, y - 1, w) + shift]
		+ begin[GetIndex(x - 1, y, w) + shift]
		+ begin[GetIndex(x, y, w) + shift]
		+ begin[GetIndex(x + 1, y, w) + shift]
		+ begin[GetIndex(x - 1, y + 1, w) + shift]
		+ begin[GetIndex(x, y + 1, w) + shift]
		+ begin[GetIndex(x + 1, y + 1, w) + shift];

	return totalColor / 9;
}

Pixel BlurPixel(int x, int y, int w, std::vector<uint8_t>::const_iterator begin)
{
	return { GetAvgColor(x, y, w, 0, begin), GetAvgColor(x, y, w, 1, begin), GetAvgColor(x, y, w, 2, begin) };
}

void BlurRect(const BMP& src, BMP& dst, int x1, int x2)
{
	int imgWidth = src.bmp_info_header.width;
	int imgHeight = src.bmp_info_header.height;

	for (int y = 1; y < imgHeight - 1; y++)
	{
		for (int x = x1; x < x2; x++)
		{
			auto [b, g, r] = BlurPixel(x, y, imgWidth, src.data.begin());
			dst.set_pixel(x, y, b, g, r, 0);
		}
	}
}

std::vector<std::pair<int, int>> SplitForRects(int w, int threadCount)
{
	std::vector<std::pair<int, int>> bounds;
	int lineW = (w - 2) / threadCount;

	int x = 1;

	for (int i = 0; i < threadCount && x + lineW < w - 1; x += lineW)
	{
		bounds.emplace_back(x, x + lineW - 1);
	}

	if (bounds.size() < threadCount)
	{
		bounds.emplace_back(x, w - 1);
	}

	return bounds;
};

int main(int argc, char* argv[])
{
	std::string inputSrc, outputSrc;
	int threadCount;
	try
	{
		auto [in, out, threads] = ParseArgs(argc, argv);
		inputSrc = in;
		outputSrc = out;
		threadCount = threads;
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what() << '\n';
	}

	BMP src(inputSrc.c_str());
	BMP dst(inputSrc.c_str());

	int w = src.bmp_info_header.width;

	auto rectBounds = SplitForRects(w, threadCount);
	for (auto [x1, x2] : rectBounds)
	{
		std::cout << x1 << " " << x2 << "\n";
	}

	std::cout << threadCount << " " << rectBounds.size() << "\n";
	const auto start = std::chrono::steady_clock::now();
	{
		std::vector<std::jthread> threads;
		for (int k = 0; k < threadCount; k++)
		{
			auto [x1, x2] = rectBounds[k];
			threads.emplace_back(BlurRect, std::cref(src), std::ref(dst), x1, x2);
		}

		std::cout << "ThreadsCount: " << threads.size() << "\n";
	}
	const auto finish = std::chrono::steady_clock::now();

	dst.write(outputSrc.c_str());
	std::cout << threadCount << ' ' << std::chrono::duration<double>(finish - start).count() << "\n";
}
