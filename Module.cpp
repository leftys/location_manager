/* 
 * File:   Module.cpp
 * Author: lefty
 * 
 * Created on 25. prosinec 2013, 12:52
 */

#include <iostream>
#include <exception>
#include <functional>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include <glog/logging.h>

#include "main.hpp"
#include "Module.hpp"
#include "Configuration.hpp"

using namespace std;

Module::Module() {
}

Module::~Module() {
}

External_module::External_module(const std::string& name) {
    command = dirname + "/ext-modules/" + name;
}
External_module::External_module(const std::string& name, const int delay_ms) 
    : External_module(name) {
    this->delay_ms = delay_ms;
}

void Module::activate() {
    active = true;
}

std::pair<std::string,int> External_module::exec(const string& cmd) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return make_pair("ERROR",-1);
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    int retval = pclose(pipe)/256;
    return make_pair(result,retval);
}

std::string External_module::get_status(const std::string& variable) {
    //LOG(INFO) << "Getting status of " << command << " cmd";
    string out;
    int retval;
    tie(out, retval) = exec(command);
    //LOG(INFO) << "Got " << out;
    if (retval != 0) throw External_module_exception(command, variable);
    vector<string> lines;
    boost::split(lines, out, boost::is_any_of("\n"));
    for (string line : lines) {
        int pos_eq = line.find('=');
        string name = line.substr(0, pos_eq);
        boost::trim(name);
        if (name == variable) {
            string value = line.substr(pos_eq + 1, line.size() - pos_eq - 1);
            boost::trim(value);
            old_status[variable] = value;
            return value;
        }
    }
    throw External_module_exception(command, variable);
}

void External_module::set(const std::string& variable, const std::string& value) {
    string out;
    int retval;
    tie(out, retval) = exec(command + " " + variable + " '" + value + "'");
    if (retval != 0) {
        LOG(WARNING) << "External command returned non-zero!";
        throw External_module_exception(command, variable);
    }
    if (out.find("SET")!=0) {
        LOG(WARNING) << "External command didn't output 'SET'!";
        throw External_module_exception(command, variable);
    }
}

void External_module::watch() {
    running = true;
    watch_thread = thread(std::bind(&External_module::thread_loop,this));
}

void External_module::watch_stop() {
    {
        lock_guard<mutex> lock(watch_mutex);
        running = false;
    }
    cv.notify_one();
}

void External_module::watch_wait() {
    watch_thread.join();
}

void External_module::thread_loop() {
    bool locked;
    do {
        for (pair<string,string> pair : old_status) {
            if (get_status(pair.first) != pair.second) {
                LOG(INFO) << "Detected change in " << command << " - variable " << pair.first;
                Configuration::reprocess();
            }
        }
        unique_lock<mutex> lock(watch_mutex);
        locked = cv.wait_for(lock, chrono::milliseconds(delay_ms), [this](){return !running;});
    } while (!locked);
    //LOG(INFO) << "Thread " << this_thread::get_id() << " ended.";
}


Module_exception::Module_exception(const string& name, 
        const string& argument) : module_name(name), variable(argument) {}

const char *Module_exception::what(void) const throw() {
    string err = "Module " + module_name +
            " can't return/set expected variable " + variable + "!";
    return err.c_str();
}

External_module_exception::External_module_exception(const string& name, 
        const string& argument) : Module_exception(name, argument) {}

const char *External_module_exception::what(void) const throw() {
    string err = "External module " + module_name +
            " didn't return/set expected variable " + variable + "!";
    return err.c_str();
}