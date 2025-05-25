#include "catch.hpp"
#include "../../../src/env/ged_env.hpp"
#include <iostream>
#include <vector>

TEST_CASE("Compute GED for Letter graphs") {
    // Initialize the GED environment
    ged::GEDEnv<ged::GXLNodeID, ged::GXLLabel, ged::GXLLabel> env;

    // Load graphs from the Letter dataset
    std::vector<ged::GEDGraph::GraphID> graph_ids = env.load_gxl_graphs(
        "../../../data/datasets/Letter/data/", "../collections/LETTER_10.xml");

    // Set edit costs
    env.set_edit_costs(ged::Options::EditCosts::LETTER);

    // Initialize the environment
    env.init(ged::Options::InitType::EAGER_WITH_SHUFFLED_COPIES);

    // Ensure at least two graphs are loaded
    REQUIRE(graph_ids.size() >= 2);

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

    // Add assertions to validate results
    REQUIRE(upper_bound >= 0.0);
    REQUIRE(runtime >= 0.0);
}