#include "../../../src/env/ged_env.hpp"
#include "../../../src/env/ged_graph.hpp"
#include "../../../src/env/common_types.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <variant>

/**********************************************************
 * The old version that uses string for all labels.
 *********************************************************/

double ged_test_with_string_labels_with_letter_dataset() {
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

    // Compute GED using a specific method (e.g., BIPARTITE)
    env.set_method(ged::Options::GEDMethod::BIPARTITE);
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

    return upper_bound;
}

ged::GEDGraph::GraphID add_gxl_graph(
    ged::GEDEnv<ged::GXLNodeID, ged::GXLLabel, ged::GXLLabel> & env,
    const std::string & name, const std::string & graph_class,
    size_t num_nodes, size_t num_edges,
    bool coords_only = true,
    unsigned int seed = 42,
    bool verbose = false
) {
    std::random_device rd;
    std::mt19937 g(seed);

	ged::GEDGraph::GraphID graph_id = env.add_graph(name, graph_class);

	// Add nodes:
	for (size_t i = 0; i < num_nodes; ++i) {
		std::string node_name = "n" + std::to_string(i);
        // Create a complex label for the node with random values:
        ged::GXLLabel node_label;

		// Add node coordinates as strings:
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        std::vector<double> coords = std::vector<double>{dist(g), dist(g)};
        // Convert to string and add to the label "x" and "y":
        node_label["x"] = std::to_string(coords[0]);
        node_label["y"] = std::to_string(coords[1]);

        // Print the node label:
        if (verbose) {
			std::cout << "Adding node: " << node_name << " with label: ";
			for (const auto & [key, value] : node_label) {
                std::cout << key << ": " << value << " ";
            }
			std::cout << std::endl;
		}

		// Add the node to the graph:
		env.add_node(graph_id, node_name, node_label);
	}

	// Add edges:
    // Get all combinations of nodes to create edges:
    std::vector<std::pair<size_t, size_t>> node_pairs;
    for (size_t i = 0; i < num_nodes; ++i) {
        for (size_t j = i + 1; j < num_nodes; ++j) {
            node_pairs.emplace_back(i, j);
        }
    }
    // Shuffle the pairs to randomize edge creation:
    std::shuffle(node_pairs.begin(), node_pairs.end(), g);
    // Create edges from the shuffled pairs:
    // Ensure we only create the specified number of edges:
    size_t edges_to_create = std::min(num_edges, node_pairs.size());
    if (verbose) {
        std::cout << "Creating " << edges_to_create << " edges from " << node_pairs.size() << " possible pairs." << std::endl;
    }
    // Iterate through the pairs and create edges:
    for (size_t i = 0; i < edges_to_create; ++i) {
        size_t src = node_pairs[i].first;
        size_t tgt = node_pairs[i].second;
        std::string edge_name = "e" + std::to_string(i);
        // Create a complex label for the edge with random values:
        ged::GXLLabel edge_label = {};
        if (verbose) {
            std::cout << "Adding edge: " << edge_name << " from node " << src << " to node " << tgt
                      << " with label: ";
            for (const auto & [key, value] : edge_label) {
                std::cout << key << ": " << value << " ";
            }
            std::cout << std::endl;
        }
        
        // Add the edge to the graph:
        env.add_edge(graph_id, "n" + std::to_string(src), "n" + std::to_string(tgt), edge_label, true);
    }

	return graph_id;
}

double ged_test_coords_labels_with_string_label_version(unsigned int seed = 42) {
    // Initialize the GED environment
    ged::GEDEnv<ged::GXLNodeID, ged::GXLLabel, ged::GXLLabel> env;

    // Add two sample graphs with complex labels:
	ged::GEDGraph::GraphID graph1_id = add_gxl_graph(env, "Graph1", "ClassA", 5, 6, true, seed, true);
	ged::GEDGraph::GraphID graph2_id = add_gxl_graph(env, "Graph2", "ClassB", 8, 12, true, seed, true);

	std::vector<ged::GEDGraph::GraphID> graph_ids = {graph1_id, graph2_id};

    std::vector<double> edit_cost_constants = {
        1.0, // Node deletion cost
        1.0, // Node insertion cost
        1.0, // Node relabeling cost
        1.0, // Edge deletion cost
        1.0, // Edge insertion cost
        1.0  // Edge relabeling cost
    };

    // Set edit costs
    env.set_edit_costs(ged::Options::EditCosts::NON_SYMBOLIC, edit_cost_constants);

    // Initialize the environment
    env.init(ged::Options::InitType::EAGER_WITH_SHUFFLED_COPIES);

    // Check if at least two graphs are loaded
    if (graph_ids.size() < 2) {
        std::cerr << "Error: Need at least two graphs loaded." << std::endl;
        return 1;
    }

    // Compute GED using a specific method (e.g., BIPARTITE)
    env.set_method(ged::Options::GEDMethod::BIPARTITE);
    env.init_method(); // Initialize the method before running it

    // Select two graphs for comparison
    ged::GEDGraph::GraphID g = graph_ids[0];
    ged::GEDGraph::GraphID h = graph_ids[1];

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

    return upper_bound; // Return the upper bound as the result
}

/*******************************************************************************
 * The version that uses complex attribute labels (vector of doubles for coordinates)
 ******************************************************************************/

ged::GEDGraph::GraphID add_attr_graph(
    ged::GEDEnv<ged::GXLNodeID, ged::AttrLabel, ged::AttrLabel> & env,
    const std::string & name, const std::string & graph_class,
    size_t num_nodes, size_t num_edges,
    bool coords_only = false,
    unsigned int seed = 42,
    bool verbose = false
) {
    std::random_device rd;
    std::mt19937 g(seed);

	ged::GEDGraph::GraphID graph_id = env.add_graph(name, graph_class);

	// Add nodes:
	for (size_t i = 0; i < num_nodes; ++i) {
		std::string node_name = "n" + std::to_string(i);
        // Create a complex label for the node with random values:
        ged::AttrLabel node_label;

		// Add node label attributes ("label": string, "coords": vector<double>, "embedding": vector<double>)
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        node_label["coords"] = std::vector<double>{dist(g), dist(g)};
        if (!coords_only) {
            node_label["label"] = "Node" + std::to_string(i);
            node_label["embedding"] = std::vector<double>{dist(g), dist(g), dist(g)};
        }

        // Print the node label:
        if (verbose) {
			std::cout << "Adding node: " << node_name << " with label: ";
			for (const auto & [key, value] : node_label) {
				if (std::holds_alternative<std::vector<double>>(value)) {
					const auto & vec = std::get<std::vector<double>>(value);
					std::cout << key << ": [";
					for (size_t j = 0; j < vec.size(); ++j) {
						std::cout << vec[j];
						if (j < vec.size() - 1) std::cout << ", ";
					}
					std::cout << "] ";
				} else if (std::holds_alternative<std::string>(value)) {
					std::cout << key << ": " << std::get<std::string>(value) << " ";
				}
			}
			std::cout << std::endl;
		}

		// Add the node to the graph:
		env.add_node(graph_id, node_name, node_label);
	}

	// Add edges:
    // Get all combinations of nodes to create edges:
    std::vector<std::pair<size_t, size_t>> node_pairs;
    for (size_t i = 0; i < num_nodes; ++i) {
        for (size_t j = i + 1; j < num_nodes; ++j) {
            node_pairs.emplace_back(i, j);
        }
    }
    // Shuffle the pairs to randomize edge creation:
    std::shuffle(node_pairs.begin(), node_pairs.end(), g);
    // Create edges from the shuffled pairs:
    // Ensure we only create the specified number of edges:
    size_t edges_to_create = std::min(num_edges, node_pairs.size());
    if (verbose) {
        std::cout << "Creating " << edges_to_create << " edges from " << node_pairs.size() << " possible pairs." << std::endl;
    }
    // Iterate through the pairs and create edges:
    for (size_t i = 0; i < edges_to_create; ++i) {
        size_t src = node_pairs[i].first;
        size_t tgt = node_pairs[i].second;
        std::string edge_name = "e" + std::to_string(i);
        // Create a complex label for the edge with random values:
        ged::AttrLabel edge_label = {};
        if (!coords_only) {
            std::uniform_real_distribution<double> weight_dist(0.0, 1.0);
            std::uniform_int_distribution<int> bond_dist(0, 2);
            edge_label["weight"] = std::vector<double>{weight_dist(g)}; // Random weight as a vector using device g
            edge_label["label"] = "bond" + std::to_string(bond_dist(g)); // Random bond type using device g
        }
        if (verbose) {
            std::cout << "Adding edge: " << edge_name << " from node " << src << " to node " << tgt
                      << " with label: ";
            for (const auto & [key, value] : edge_label) {
                if (std::holds_alternative<double>(value)) {
                    std::cout << key << ": " << std::get<double>(value) << " ";
                } else if (std::holds_alternative<std::string>(value)) {
                    std::cout << key << ": " << std::get<std::string>(value) << " ";
                }
            }
            std::cout << std::endl;
        }
        
        // Add the edge to the graph:
        env.add_edge(graph_id, "n" + std::to_string(src), "n" + std::to_string(tgt), edge_label, true);
    }

	return graph_id;
}


double ged_test_coords_labels_with_attr_label_version(unsigned int seed = 42) {
	// Initialize the GED environment
    ged::GEDEnv<ged::GXLNodeID, ged::AttrLabel, ged::AttrLabel> env;

    // Add two sample graphs with complex labels:
	ged::GEDGraph::GraphID graph1_id = add_attr_graph(env, "Graph1", "ClassA", 5, 6, true, seed, true);
	ged::GEDGraph::GraphID graph2_id = add_attr_graph(env, "Graph2", "ClassB", 8, 12, true, seed, true);

	std::vector<ged::GEDGraph::GraphID> graph_ids = {graph1_id, graph2_id};

    // Set similarity metrics:
    std::unordered_map<std::string, std::any> edit_cost_config = {
        {"node_coord_metric", "euclidean"},  // Example: Use Euclidean distance for node coordinates
    };
    std::vector<double> edit_cost_constants = {
        1.0, // Node deletion cost
        1.0, // Node insertion cost
        1.0, // Node relabeling cost
        1.0, // Edge deletion cost
        1.0, // Edge insertion cost
        1.0  // Edge relabeling cost
    };

    // Set edit costs
    env.set_edit_costs(ged::Options::EditCosts::GEOMETRIC, edit_cost_constants, edit_cost_config);

    // Initialize the environment
    env.init(ged::Options::InitType::EAGER_WITHOUT_SHUFFLED_COPIES);

    // Check if at least two graphs are loaded
    if (graph_ids.size() < 2) {
        std::cerr << "Error: Need at least two graphs loaded." << std::endl;
        return 1;
    }

    // Compute GED using a specific method (e.g., BIPARTITE)
    env.set_method(ged::Options::GEDMethod::BIPARTITE);
    env.init_method(); // Initialize the method before running it

    // Select two graphs for comparison
    ged::GEDGraph::GraphID g = graph_ids[0];
    ged::GEDGraph::GraphID h = graph_ids[1];

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

    return upper_bound;
}

double ged_test_with_complex_labels() {
	// Initialize the GED environment
    ged::GEDEnv<ged::GXLNodeID, ged::AttrLabel, ged::AttrLabel> env;

    // Add two sample graphs with complex labels:
	ged::GEDGraph::GraphID graph1_id = add_attr_graph(env, "Graph1", "ClassA", 5, 6, false, true);
	ged::GEDGraph::GraphID graph2_id = add_attr_graph(env, "Graph2", "ClassB", 8, 12, false, true);

	std::vector<ged::GEDGraph::GraphID> graph_ids = {graph1_id, graph2_id};

    // Set similarity metrics:
    std::unordered_map<std::string, std::any> edit_cost_config = {
        {"node_coord_metric", "euclidean"},  // Example: Use Euclidean distance for node coordinates
        {"node_embed_metric", "cosine_distance"},    // Example: Use cosine similarity for node embeddings
        {"edge_weight_metric", "euclidean"}, // Example: Use Euclidean distance for edge weights
        {"normalize_cosine", true}, // Normalize cosine similarity
    };
    std::vector<double> edit_cost_constants = {
        1.0, // Node deletion cost
        1.0, // Node insertion cost
        1.0, // Node relabeling cost
        1.0, // Edge deletion cost
        1.0, // Edge insertion cost
        1.0  // Edge relabeling cost
    };

    // Set edit costs
    env.set_edit_costs(ged::Options::EditCosts::GEOMETRIC, edit_cost_constants, edit_cost_config);

    // Initialize the environment
    env.init(ged::Options::InitType::EAGER_WITHOUT_SHUFFLED_COPIES);

    // Check if at least two graphs are loaded
    if (graph_ids.size() < 2) {
        std::cerr << "Error: Need at least two graphs loaded." << std::endl;
        return 1;
    }

    // Compute GED using a specific method (e.g., BIPARTITE)
    env.set_method(ged::Options::GEDMethod::BIPARTITE);
    env.init_method(); // Initialize the method before running it

    // Select two graphs for comparison
    ged::GEDGraph::GraphID g = graph_ids[0];
    ged::GEDGraph::GraphID h = graph_ids[1];

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

    return upper_bound;
}

/*******************************************************************************
 * Compare string and attribute label versions for coordinates
 ******************************************************************************/

bool ged_test_compare_string_and_attr_label_versions_for_coords(
    unsigned int seed = 42
) {
    double cost_string_version = ged_test_coords_labels_with_string_label_version(seed);
    double cost_attr_version = ged_test_coords_labels_with_attr_label_version(seed);
    std::cout << "Cost of string label version: " << cost_string_version << std::endl;  
    std::cout << "Cost of attribute label version: " << cost_attr_version << std::endl;
    if (std::abs(cost_string_version - cost_attr_version) > 1e-6) {
        std::cout << "The costs are different!" << std::endl;
        return false;
    } else {
        std::cout << "The costs are the same!" << std::endl;
        return true;
    }
}

int main() {
    unsigned int seed = 42; // Set a fixed seed for reproducibility
	// ged_test_with_string_labels_with_letter_dataset();
	// ged_test_with_complex_labels();
    // ged_test_coords_labels_with_string_label_version(seed);
    // ged_test_coords_labels_with_attr_label_version(seed);
    ged_test_compare_string_and_attr_label_versions_for_coords(seed);
	return 0;
}
