#include <iostream>
#include <thread>
#include <vector>

int GetThreadCount(int argc, char* argv[])
{
	if (argc < 2)
	{
		throw std::invalid_argument("Provide count of threads as first argument");
	}

	int threadCount;
	try
	{
		threadCount = std::stoi(argv[1]);
	}
	catch (const std::logic_error& e)
	{
		throw std::invalid_argument("Threads count should be valid integer");
	}

	if (threadCount <= 0)
	{
		throw std::invalid_argument("Threads count should be positive number");
	}

	return threadCount;
}

void Worker(int index)
{
	std::cout << std::format("I am {} thread\n", index);
}

int main(int argc, char* argv[])
{
	int threadCount;
	try
	{
		threadCount = GetThreadCount(argc, argv);
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what() << '\n';
		return 1;
	}

	std::vector<std::jthread> threads;
	threads.reserve(threadCount);

	for (int i = 1; i <= threadCount; ++i)
	{
		threads.emplace_back(Worker, i);
	}
}
