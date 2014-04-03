/* 
 * File:   main.cpp
 * Author: lefty
 *
 * Created on 23. prosinec 2013, 15:17
 */

#include <csignal>
#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <glog/logging.h>

#include "main.hpp"
#include "Configuration.hpp"
#include "Module.hpp"
#include "Module_manager.hpp"
#include "Module_usb.hpp"
#include "Module_monitor.hpp"

using namespace std;
namespace po = boost::program_options;

Module_manager mod_mgr;

/**
 * @brief Unix process signal handler 
 * @param sig signal number
 */
void handle_signal(int sig) {
    LOG(WARNING) << "Signal '" << strsignal(sig) << "' caught.";
    mod_mgr.watch_stop();
}

/**
 * @brief Processes command-line arguments
 */
po::variables_map process_args(int argc, char **argv) {
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("cfg,C", po::value<string>()->default_value("~/.location_manager.cfg"), "path to config file")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        exit(1);
    }
    return std::move(vm);
}

string dirname;

/**
 * @brief Entry point
 */
int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::WARNING); // Log info-level messages to stderr
    LOG(WARNING) << "Location manager v" << version << " init";
    dirname = boost::filesystem::path(argv[0]).parent_path().string();
    LOG(INFO) << "Will be looking for ext-modules and helpers directories in " << dirname;
    po::variables_map vm = process_args(argc, argv);
    
    mod_mgr.register_module("xrandr",new External_module("xrandr"));
    mod_mgr.register_module("execute",new External_module("execute"));
    mod_mgr.register_module("wifi",new External_module("wifi"));
    mod_mgr.register_module("wifi_scan",new External_module("wifi_scan",30000));
    mod_mgr.register_module("usb",new Module_usb());
    mod_mgr.register_module("monitor",new Module_monitor());
    
    LOG(INFO) << "Using config file " << vm["cfg"].as<string>();
    Configuration cfg (vm["cfg"].as<string>());
    mod_mgr.watch();
    Configuration::reprocess();
    
    signal(SIGTERM, &handle_signal);
    signal(SIGINT, &handle_signal);
    signal(SIGABRT, &handle_signal);
    
    mod_mgr.join();
    return 0;
}

