/* 
 * File:   Cfg_item.cpp
 * Author: lefty
 * 
 * Created on 23. prosinec 2013, 15:59
 */

#include <boost/algorithm/string.hpp>
#include <string>
#include <algorithm>
#include <functional>
#include <glog/logging.h>

#include "Cfg_item.hpp"
#include "Module.hpp"
#include "Configuration.hpp"
#include "Term_evaluation.hpp"

using namespace std;

Module_manager *Cfg_item::module_manager = nullptr;

Cfg_item::Cfg_item(string& line) {
    int pos_eq = line.find('=');
    name = line.substr(0,pos_eq);
    value = line.substr(pos_eq+1, line.size()-pos_eq-1);
    boost::algorithm::trim(name);
    boost::algorithm::trim(value);
}

string& Cfg_item::get_name() {
    return name;
}

Cfg_item::~Cfg_item() {
}

Cfg_variable::Cfg_variable(string& line) : Cfg_item(line) {
    vector<string> str_vec;
    boost::split(str_vec, value, boost::is_any_of(".="));
    module = Cfg_item::module_manager->get_module(str_vec[0]);
    if (!module) {
        throw Referenced_element_missing(
                "Module "+str_vec[0]+" not found. Its name is probably misspeled.");
    }
    module_argument = str_vec[1];
    module_value = str_vec[2];
    boost::algorithm::trim(module_argument);
    boost::algorithm::trim(module_value);
    
    module->activate();
}

bool Cfg_variable::get_value() {
    string value = module->get_status(module_argument);
    vector<string> values;
    boost::split(values, value, boost::is_any_of(","));
    for (string got : values) {
        boost::trim(got);
        if (module_value == got) return true;
    }
    return false;
}

void Cfg_variable::set_value() {
    module->set(module_argument,module_value);
}

void Cfg_variable::reset_value() {
    module->set(module_argument,"reset");
}
Cfg_location::Cfg_location(std::string &line) : Cfg_item(line),was_here(false) {
}

bool Cfg_location::here(void) {
    Term_evaluation term (value);
    function<bool(string&)> fcn = std::bind(&Cfg_location::eval_variable,this,placeholders::_1);
    bool is_here = term.eval(fcn);
    //no_run++;
    //LOG(WARNING) << name << "@" << this << " " << was_here << " -> " << is_here 
    //        << "(" << no_run << ")";
    try {
        Cfg_rule& rule (Configuration::rules.at(name));
        if (!was_here && is_here) 
            rule.commit();
        if (was_here && !is_here) 
            rule.rollback();
        was_here = is_here;
    } catch (out_of_range ex) {
        throw Referenced_element_missing(
            "Rule " + name + " referenced from that location doesn't exist!");
    }
    return is_here;
}

bool Cfg_location::eval_variable(std::string &var_name) {
    try {
        bool value = Configuration::senses.at(var_name).get_value();
        LOG(INFO) << "Sense " << var_name << " is " << value;
        return value;
    } catch (out_of_range ex) {
        throw Referenced_element_missing(
                "Variable " + var_name + " doesn't exist! You can declare it in [senses]");
    }
}

Cfg_rule::Cfg_rule(std::string &line) : Cfg_item(line) {
    vector<string> str_vec;
    boost::split(str_vec, value, boost::is_any_of(","));
    dependencies.reserve(str_vec.size());
    for (auto action_name : str_vec) {
        if (Configuration::actions.count(action_name) == 0)
            throw Referenced_element_missing(
                "Action " + action_name + " doesn't exist! You can declare it in [actions]");
        Cfg_variable *item = &(Configuration::actions.at(action_name));
        dependencies.push_back(item);
    }
}

void Cfg_rule::commit(void) {
    LOG(WARNING) << "Running " << name;
    for (Cfg_variable* action : dependencies) {
        action->set_value();
    }
}

void Cfg_rule::rollback(void) {
    LOG(INFO) << "Rolling-back " << name;
    for (Cfg_variable* action : dependencies) {
        action->reset_value();
    }
}
