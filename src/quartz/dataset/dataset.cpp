#include "dataset.h"

#include <cassert>
#include <fstream>
#include <iomanip>

namespace quartz {
	int Dataset::num_hash_values() const { return (int)dataset.size(); }

	int Dataset::num_total_dags() const {
		int ret = 0;
		for (const auto &it : dataset) {
			ret += (int)it.second.size();
		}
		return ret;
	}

	bool Dataset::save_json(Context *ctx, const std::string &file_name) const {
		std::ofstream fout;
		fout.open(file_name, std::ofstream::out);
		if (!fout.is_open()) {
			return false;
		}

		fout << "[" << std::endl;

		// The generated parameters for random testing.
		auto all_parameters = ctx->get_all_generated_parameters();
		fout << "[";
		bool start0 = true;
		for (auto &param : all_parameters) {
			if (start0) {
				start0 = false;
			}
			else {
				fout << ", ";
			}
			fout << std::scientific << std::setprecision(17) << param;
		}
		fout << "]," << std::endl;

		fout << "{" << std::endl;
		start0 = true;
		for (const auto &it : dataset) {
			if (it.second.empty()) {
				// Empty DAG set
				continue;
			}
			if (start0) {
				start0 = false;
			}
			else {
				fout << ",";
			}
			fout << "\"" << std::hex << it.first << "\": [" << std::endl;
			bool start = true;
			for (const auto &dag : it.second) {
				if (start) {
					start = false;
				}
				else {
					fout << ",";
				}
				fout << dag->to_json();
			}
			fout << "]" << std::endl;
		}
		fout << "}" << std::endl;

		fout << "]" << std::endl;
		return true;
	}

	int Dataset::remove_singletons(Context *ctx) {
		int num_removed = 0;
		for (auto it = dataset.begin(); it != dataset.end();) {
			if (it->second.size() != 1) {
				it++;
				continue;
			}
			auto &dag = it->second[0];
			auto it_hash_value = dag->hash(ctx);
			bool found_possible_equivalence = false;
			for (auto &hash_value : dag->other_hash_values()) {
				auto find_other = dataset.find(hash_value);
				if (find_other != dataset.end() &&
				    !find_other->second.empty()) {
					found_possible_equivalence = true;
					break;
				}
				assert(hash_value ==
				       it_hash_value + 1); // Only deal with this case...
			}
			// ...so that we know for sure that only DAGs with hash value equal
			// to |it_hash_value - 1| can have other_hash_values() containing
			// |it_hash_value|.
			auto find_other = dataset.find(it_hash_value - 1);
			if (find_other != dataset.end() && !find_other->second.empty()) {
				found_possible_equivalence = true;
				break;
			}
			if (found_possible_equivalence) {
				it++;
				continue;
			}
			// Remove |it|.
			auto remove_it = it;
			it++;
			dataset.erase(remove_it);
			num_removed++;
		}
		return num_removed;
	}

	int Dataset::normalize_to_minimal_circuit_representations(Context *ctx) {
		int num_removed = 0;
		std::vector< std::unique_ptr< DAG > > dags_to_insert_afterwards;
		auto dag_already_exists =
		    [](const DAG &dag,
		       const std::vector< std::unique_ptr< DAG > > &new_dags) {
			    for (auto &other_dag : new_dags) {
				    if (dag.fully_equivalent(*other_dag)) {
					    return true;
				    }
			    }
			    return false;
		    };

		for (auto &item : dataset) {
			auto &current_hash_value = item.first;
			auto &dags = item.second;
			auto size_before = dags.size();
			std::vector< std::unique_ptr< DAG > > new_dags;
			std::unique_ptr< DAG > new_dag;

			for (auto &dag : dags) {
				bool is_minimal = dag->minimal_circuit_representation(&new_dag);
				if (!is_minimal) {
					if (!dag_already_exists(*new_dag, new_dags)) {
						new_dags.push_back(std::move(new_dag));
					}
					dag = nullptr; // delete the original DAG
				}
			}
			if (!new_dags.empty()) {
				// |item| is modified.
				for (auto &dag : dags) {
					// Put all dags into |new_dags|.
					if (dag != nullptr) {
						if (!dag_already_exists(*dag, new_dags)) {
							new_dags.push_back(std::move(dag));
						}
					}
				}
				// Update |dags|.
				dags.clear();
				for (auto &dag : new_dags) {
					const auto hash_value = dag->hash(ctx);
					if (hash_value == current_hash_value) {
						dags.push_back(std::move(dag));
					}
					else {
						// The hash value changed due to floating-point errors.
						// Insert |dag| later to avoid corrupting the iterator
						// of |dataset|.
						dags_to_insert_afterwards.push_back(std::move(dag));
					}
				}
				auto size_after = dags.size();
				num_removed += (int)(size_before - size_after);
			}
		}
		for (auto &dag : dags_to_insert_afterwards) {
			const auto hash_value = dag->hash(ctx);
			if (!dag_already_exists(*dag, dataset[hash_value])) {
				num_removed--; // Insert |dag| back.
				dataset[hash_value].push_back(std::move(dag));
			}
		}
		return num_removed;
	}

	bool Dataset::insert(Context *ctx, std::unique_ptr< DAG > dag) {
		const auto hash_value = dag->hash(ctx);
		bool ret = dataset.count(hash_value) == 0;
		dataset[hash_value].push_back(std::move(dag));
		return ret;
	}

	void Dataset::clear() { dataset.clear(); }

} // namespace quartz