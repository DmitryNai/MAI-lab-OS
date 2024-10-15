#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

struct ThreadData {
    unsigned long long points_in_circle;
    unsigned long long total_points;
    double radius;
};

void* monteCarlo(void* arg) {
    ThreadData* data = reinterpret_cast<ThreadData*>(arg);
    unsigned long long points_in_circle = 0;

    for (unsigned long long i = 0; i < data->total_points; ++i) {
        double x = static_cast<double>(rand()) / RAND_MAX * data->radius;
        double y = static_cast<double>(rand()) / RAND_MAX * data->radius;
        if (x * x + y * y <= data->radius * data->radius) {
            ++points_in_circle;
        }
    }

    data->points_in_circle = points_in_circle;
    pthread_exit(nullptr);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <radius> <number_of_threads> <total_points>\n";
        return 1;
    }

    double radius = std::stod(argv[1]);
    int num_threads = std::stoi(argv[2]);
    unsigned long long total_points = std::stoull(argv[3]);

    srand(static_cast<unsigned int>(time(nullptr)));

    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> thread_data(num_threads);

    unsigned long long points_per_thread = total_points / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].total_points = points_per_thread;
        thread_data[i].radius = radius;
        pthread_create(&threads[i], nullptr, monteCarlo, &thread_data[i]);
    }

    unsigned long long total_in_circle = 0;
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
        total_in_circle += thread_data[i].points_in_circle;
    }
    double area = (static_cast<double>(total_in_circle) / total_points) * (radius * radius) * 4.0;

    std::cout << "Estimated area of the circle: " << area << std::endl;

    return 0;
}
