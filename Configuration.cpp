/* 
 * File:   Configuration.cpp
 * Author: lefty
 * 
 * Created on 23. prosinec 2013, 15:38
 */

#include <iostream>
#include <fstream>
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>

#include "Configuration.hpp"

using namespace std;

//Static variable initialization
mutex Configuration::reprocess_mutex;

Configuration::Configuration(string cfg_file_name) {
    ifstream cfg_file;
    cfg_file.exceptions(ios::failbit | ios::badbit);
    try {
        cfg_file.open(cfg_file_name);
        while (!cfg_file.eof()) {
            string line;
            getline(cfg_file,line);
            handle_line(line);
        }
    } catch (ios::failure ex) {
        if (cfg_file.eofbit) return;
        
        LOG(FATAL) << "IO Error reading cfg file:" << ex.what();
        cfg_file.close();
        exit(1);
    }
}

void Configuration::reprocess() {
    lock_guard<mutex> lock(reprocess_mutex);
    for (auto &loc_pair : Configuration::locations) {
        Cfg_location& loc = loc_pair.second;
        bool res = loc.here();
        VLOG(1) << loc.get_name() << ": " << res;
    }
}

template <class Cfg_item_t>
void add_action(map<string,Cfg_item_t>* V, string& line) {
    Cfg_item_t item = Cfg_item_t(line);
    string name = item.get_name();
    LOG(INFO) << "  adding " << name;
    V->insert(std::pair<string,Cfg_item_t>(name,std::move(item)));
}

// Definition of static variables:
std::map<std::string, Cfg_variable> Configuration::senses, Configuration::actions;
std::map<std::string, Cfg_location> Configuration::locations;
std::map<std::string, Cfg_rule> Configuration::rules;

void Configuration::handle_line(string& line) {
    boost::algorithm::trim(line);
    if (line == "") 
        return;
    else if (line[0] == '#') 
        return; //Ignore comments
    else if (line[0] == '[') { //Section header
        size_t right_bracket_pos = line.find(']');
        section = string(line,1,right_bracket_pos-1);
        if (section=="actions")
            section_method = std::bind(add_action<Cfg_variable>, &actions, std::placeholders::_1);
        if (section=="senses")
            section_method = std::bind(add_action<Cfg_variable>, &senses, std::placeholders::_1);
        if (section=="locations")
            section_method = std::bind(add_action<Cfg_location>, &locations, std::placeholders::_1);
        if (section=="rules")
            section_method = std::bind(add_action<Cfg_rule>, &rules, std::placeholders::_1);
        LOG(INFO) << "Entering section " << section;
        return;
    } else {
        section_method(line);
    }
}

Referenced_element_missing::Referenced_element_missing(const string& message) : message(message) {
}

const char *Referenced_element_missing::what(void) const throw() {
    return message.c_str();
}