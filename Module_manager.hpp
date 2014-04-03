/* 
 * File:   Module_manager.hpp
 * Author: lefty
 *
 * Created on 25. prosinec 2013, 17:30
 */

#ifndef MODULE_MANAGER_HPP
#define	MODULE_MANAGER_HPP

#include <string>
#include <map>

#include "Module.hpp"

class Module_manager {
public:
    Module_manager();
    Module_manager(const Module_manager& orig) = delete;
    virtual ~Module_manager();
    
    void register_module(const std::string& name, Module *module);
    Module *get_module(std::string& name);
    void watch(void);
    void watch_stop(void);
    void join(void);
private:
    bool file_exists(const std::string& fileName);
    std::map<std::string,Module*> module_map;
};

#endif	/* MODULE_MANAGER_HPP */

