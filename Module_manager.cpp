/* 
 * File:   Module_manager.cpp
 * Author: lefty
 * 
 * Created on 25. prosinec 2013, 17:30
 */

#include <fstream>
#include <glog/logging.h>

#include "Module_manager.hpp"
#include "Cfg_item.hpp"

using namespace std;

Module_manager::Module_manager() {
    Cfg_item::module_manager = this;
}

Module_manager::~Module_manager() {
    for (auto module_pair : module_map) {
        delete module_pair.second;
    }
    Cfg_item::module_manager = nullptr;
}

Module *Module_manager::get_module(string& name) {
    if (module_map.count(name)==0) { // Automatically initialize referenced external modules
        if (file_exists("./ext-modules/" + name)) {
            LOG(INFO) << "Found and registered external module " << name;
            register_module(name,new External_module(name));
        }
    }
    return module_map[name];
}

void Module_manager::register_module(const string& name, Module *module) {
    module_map[name]=module;
}

void Module_manager::watch() {
    for (pair<string,Module*> module : module_map) {
        module.second->watch();
    }
}

void Module_manager::watch_stop() {
    for (pair<string,Module*> module : module_map) {
        module.second->watch_stop();
    }
}

void Module_manager::join() {
    for (pair<string,Module*> module : module_map) {
        module.second->watch_wait();
    }
}

bool Module_manager::file_exists(const string& fileName)
{
    ifstream infile(fileName);
    return infile.good();
}