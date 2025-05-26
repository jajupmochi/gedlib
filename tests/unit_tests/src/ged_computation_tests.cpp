#include "../../../src/env/ged_env.hpp"
#include <iostream>
#include <string>
#include <vector>

int main() {
    // Get the absolute path to the current directory:
    std::string current_path = __FILE__;
    current_path = current_path.substr(0, current_path.find_last_of("/\\"));
    std::cout << "Current path: " << current_path << std::endl;
    // Get the absolute path to the data directory:
    std::string data_path = current_path + "/../../../data/datasets/Letter/HIGH/";
    std::cout << "Data path: " << data_path << std::endl;
    // Get the absolute path to the collection file: 
    std::string collection_path = current_path + "/../collections/Letter_test.xml";
    std::cout << "Collection path: " << collection_path << std::endl;

    // Initialize the GED environment
    ged::GEDEnv<ged::GXLNodeID, ged::GXLLabel, ged::GXLLabel> env;

    // Load graphs from the Letter dataset
    std::vector<ged::GEDGraph::GraphID> graph_ids = env.load_gxl_graphs(
        data_path, collection_path);
//        "../../../data/datasets/Letter/data/", "../collections/Letter_test.xml");

    // Set edit costs
    env.set_edit_costs(ged::Options::EditCosts::LETTER);

    // Initialize the environment
    env.init(ged::Options::InitType::EAGER_WITH_SHUFFLED_COPIES);

    // Check if at least two graphs are loaded
    if (graph_ids.size() < 2) {
        std::cerr << "Error: Need at least two graphs loaded." << std::endl;
        return 1;
    }

    // Select two graphs for comparison
    ged::GEDGraph::GraphID g = graph_ids[0];
    ged::GEDGraph::GraphID h = graph_ids[1];

    // Compute GED using a specific method (e.g., HED)
    env.set_method(ged::Options::GEDMethod::HED);
    env.run_method(g, h);

    // Retrieve and print results
    double upper_bound = env.get_upper_bound(g, h);
    double runtime = env.get_runtime(g, h);

    std::cout << "GED between graph " << g << " and graph " << h << ":\n";
    std::cout << "Upper bound: " << upper_bound << "\n";
    std::cout << "Runtime: " << runtime << " seconds\n";

    // Simple sanity check
    if (upper_bound < 0.0 || runtime < 0.0) {
        std::cerr << "Warning: Unexpected result values." << std::endl;
        return 1;
    }

    return 0;
}
