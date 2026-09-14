#include <charconv>
#include <print>
#include <thread>
#include <vector>
#include <system_error>
#include <mutex>
#include <cstring>

std::mutex print_mutex;

void Worker(int index)
{
    std::lock_guard<std::mutex> lock(print_mutex);
    std::println("Поток № {} выполняет свою работу", index);
}

int main(int argc, char* argv[])
{
    // ...разбор N из argv
    if (argc != 2)
    {
        std::println(stderr, "Программа должна быть запущена с одним аргументом - количеством потоков N (N > 0)");
        return 1;
    }

    int threadCount;
    auto [ptr, ec] = std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), threadCount);

    if (ec == std::errc::invalid_argument)
    {
        std::println(stderr, "Аргумент {} не является числом.", argv[1]);
        return 1;
    }

    if (threadCount <= 0)
    {
        std::println(stderr, "Количество потоков должно быть больше 0, получено {}", threadCount);
        return 1;
    }


    std::vector<std::jthread> threads;
    threads.reserve(threadCount);
    for (int i = 1; i <= threadCount; ++i)
    {
        threads.emplace_back(Worker, i);
    }

    // join происходит в деструкторе jthread
    return 0;
}
