#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <vector>

template <typename T> class ThreadSafeVector {
  private:
	std::vector<T> data;
	mutable std::mutex vecMutex;

  public:
	// Insert a new element in a thread-safe manner
	void push_back(const T &value) {
		std::lock_guard<std::mutex> lock(vecMutex);
		data.push_back(value);
	}

	// Get the maximum element in the vector in a thread-safe manner
	T get_max() const {
		if (data.empty()) {
			return std::numeric_limits<T>::min();
		}
		std::lock_guard<std::mutex> lock(vecMutex);
		if (data.empty()) {
			throw std::runtime_error("Vector is empty");
		}
		return *std::max_element(data.begin(), data.end());
	}

	void print_max() const {
		if (data.empty()) {
			return;
		}
		std::lock_guard<std::mutex> lock(vecMutex);
		if (data.empty()) {
			return;
		}
		std::cout << "Max: " << *std::max_element(data.begin(), data.end())
				  << std::endl;
	}

	// Get the percentile element in a thread-safe manner
	T get_percentile(double percentile) const {
		std::lock_guard<std::mutex> lock(vecMutex);
		if (data.empty()) {
			throw std::runtime_error("Vector is empty");
		}
		if (percentile < 0.0 || percentile > 100.0) {
			throw std::invalid_argument("Percentile must be between 0 and 100");
		}

		// Make a copy of the data and sort it to find the percentile
		std::vector<T> sortedData = data;
		std::sort(sortedData.begin(), sortedData.end());

		// Calculate the index for the percentile
		size_t index =
			static_cast<size_t>((percentile / 100.0) * (sortedData.size() - 1));
		return sortedData[index];
	}

	void print_percentile(double percentile) const {
		std::lock_guard<std::mutex> lock(vecMutex);
		if (data.empty()) {
			return;
		}
		if (percentile < 0.0 || percentile > 100.0) {
			return;
		}

		// Make a copy of the data and sort it to find the percentile
		std::vector<T> sortedData = data;
		std::sort(sortedData.begin(), sortedData.end());

		// Calculate the index for the percentile
		size_t index =
			static_cast<size_t>((percentile / 100.0) * (sortedData.size() - 1));
		std::cout << "Percentile " << percentile << ": " << sortedData[index]
				  << std::endl;
	}

	void print_percentiles() {
		this->print_percentile(50);
		this->print_percentile(90);
		this->print_percentile(99);
		this->print_percentile(99.9);
		this->print_percentile(99.99);
		this->print_max();
	}

	// Get the size of the vector (for testing purposes)
	size_t size() const {
		std::lock_guard<std::mutex> lock(vecMutex);
		return data.size();
	}

	// Save the contents of the vector in increasing order to a CSV file
	// void save_to_csv(const std::string& filename) const {
	//     std::lock_guard<std::mutex> lock(vecMutex);

	//     // Make a copy of the data and sort it
	//     std::vector<T> sortedData = data;
	//     std::sort(sortedData.begin(), sortedData.end());

	//     // Write to the CSV file
	//     std::ofstream file(filename);
	//     if (!file.is_open()) {
	//         throw std::runtime_error("Failed to open file: " + filename);
	//     }

	//     for (const auto& value : sortedData) {
	//         file << value << "\n";
	//     }

	//     file.close();
	// }
};