
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include "toptw/huq.h"
#include "toptw/ProblemToptw.hpp"
#include "toptw/AlgoParameter.hpp"
#include "toptw/VNS.hpp"

void preparePaths(AlgoParameter &param);
void getAllInstancePaths(const AlgoParameter &param, std::vector<std::string> &paths);
std::map<std::string, std::vector<int>> readToptwBksINST(const std::string &path);

int main(int argc, char *argv[]) {
    AlgoParameter param(
            "654321", // todo: your student id
            "Qian", // todo: your name
            "toptw",          // problem_name
            "VNS", // algo_name
            60,             // time_limit = 1800 seconds
            "pr", ".txt",     // teston_prefix, teston_extension
            "../data/toptw",      // path_data
            3, // random seed
            true // debug
    );
    preparePaths(param);
    std::cout << param << std::endl;

    // read benchmark best know solutions
    std::string pathBksInst = "../data/toptw/bks.csv";
    std::map<std::string, std::vector<int>> bksInst = readToptwBksINST(pathBksInst);
    for (auto &bks: bksInst) {
        huq::println_tab("  best known solution", bks.first, huq::join(bks.second));
    }

    std::vector<std::string> paths;
    getAllInstancePaths(param, paths);
    for (auto &path: paths) {
//        huq::println_tab(path);
        std::filesystem::path file = path;
        ProblemToptw inst;
        inst.instname = file.filename();
        inst.loadInstance(path);
        inst.prepare();
        for (int m = 1; m <= 4; ++m) {
            std::string key = inst.instname.substr(0, inst.instname.find_first_of("."));
            int bks = bksInst[key][m];
            inst.setNumOfVehicles(m, bks);
            std::cout << "\r\n  Solving " << inst.instname << " with m = " << inst.m << " and BKS = " << bks
                      << " > \r\n";

            VNS algo(inst, param);
            algo.run();
        }
    }

    return 0;
}


void preparePaths(AlgoParameter &param) { // require c++17 support
//    std::cout << "dir = " << std::filesystem::current_path() << std::endl;
    std::filesystem::path dir_result = "../result";
    if (!std::filesystem::exists(dir_result) || !std::filesystem::is_directory(dir_result)) {
        std::filesystem::create_directory(dir_result);
    }
    std::filesystem::path dir_problem = "../result/" + param.problem_name;
    if (!std::filesystem::exists(dir_problem) || !std::filesystem::is_directory(dir_problem)) {
        std::filesystem::create_directory(dir_problem);
    }
    std::filesystem::path dir_algo = dir_problem.string() + "/" + param.algo_name;
    if (!std::filesystem::exists(dir_algo) || !std::filesystem::is_directory(dir_algo)) {
        std::filesystem::create_directory(dir_algo);
    }

//    std::cout << "dir_problem = " << dir_problem.string() << std::endl;
//    std::cout << "dir_algo = " << dir_algo.string() << std::endl;
    param.path_result_sol = dir_algo.string();
    param.path_result_csv = dir_problem.string() + "/" + param.csv_name();

    std::filesystem::path csv = param.path_result_csv;
    if (!std::filesystem::exists(csv)) {
        // initial table in result csv
        std::ofstream out(param.path_result_csv, std::ofstream::out);
        out << "authorId,authorName,problemName,instance,N,m,BKS,algoName,solution,time" << std::endl;
        out.close();
    }
}

void filter_paths(const std::vector<std::string> &folders, bool test_on, const std::string &test_prefix,
                  std::vector<std::filesystem::path> &paths) {
    for (const std::string &folder: folders) {
        std::filesystem::path dir_data = folder;
        for (auto &p: std::filesystem::recursive_directory_iterator(dir_data)) {
            if (!test_on || (test_on && p.path().filename().string().starts_with(test_prefix))) {
                paths.push_back(p.path());
            }
        }
    }
    std::sort(paths.begin(), paths.end());
}

void getAllInstancePaths(const AlgoParameter &param, std::vector<std::string> &paths) {
    std::filesystem::path dir_data = param.path_data;
    for (auto &p: std::filesystem::recursive_directory_iterator(dir_data)) {
//    for (auto &p: std::filesystem::directory_iterator(dir_data)) {
        std::filesystem::path file = p.path();
        std::string pathname = p.path().string();
        std::string filename = p.path().filename();
        bool match = p.path().extension() == param.test_extension;
        if (match && param.test_prefix.length() > 0) {
            match = filename.find(param.test_prefix, 0) == 0;
        }
        if (match) {
            paths.push_back(pathname);
        }
    }
    std::sort(paths.begin(), paths.end());

}


std::map<std::string, std::vector<int>> readToptwBksINST(const std::string &pathBksInst) {
    std::map<std::string, std::vector<int>> bksInst;
    std::ifstream fin(pathBksInst);
    if (!fin.is_open()) {
        std::cerr << "File open failed! " << pathBksInst << std::endl;
        std::exit(-1);
    }
    std::string line;
    fin >> line >> line;
    // while (fin.peek() != EOF) {
    while (fin.good()) {
        int i1 = line.find_first_of(",");
        int i2 = line.find_last_of(",");
        std::string key = line.substr(0, i1);
        int m = std::stoi(line.substr(i1 + 1, i2 - i1 - 1));
        int v = std::stoi(line.substr(i2 + 1));
        if (bksInst.find(key) == bksInst.end()) {
            bksInst[key] = std::vector<int>(5, 0);
        }
        bksInst[key][m] = v;
        // std::cout << line << "\t" << m << "\t" << huq::join(bksInst[key]) << std::endl;
        fin >> line;
    }
    return bksInst;
}
