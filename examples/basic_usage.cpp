#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>
#include "stopwatch.hpp"

using namespace std;
namespace ch = chrono;
using namespace chrono_literals;

using seconds = ch::duration<long double, ratio<1>>;

int main() {
	stopwatch sw;
	auto start = sw.now();

	cout << fixed << setprecision(2);

	cout << "Time is -1x real time\n";
	sw.set_speed(-1);
	for (int i = 0; i < 5; i++) {
		cout << ch::duration_cast<seconds>(sw.now().time_since_epoch()).count() << " seconds\n";
		this_thread::sleep_for(1s);
	}

	cout << "Time is 2.5x real time\n";
	sw.set_speed(2.5);
	for (int i = 0; i < 5; i++) {
		cout << ch::duration_cast<seconds>(sw.now().time_since_epoch()).count() << " seconds\n";
		this_thread::sleep_for(1s);
	}

	cout << "Time is 0x real time\n";
	sw.set_speed(0);
	for (int i = 0; i < 5; i++) {
		cout << ch::duration_cast<seconds>(sw.now().time_since_epoch()).count() << " seconds\n";
		this_thread::sleep_for(1s);
	}

	auto finish = sw.now();
	cout << "From start to finish: " << ch::duration_cast<seconds>(finish - start).count() << " seconds\n";
}