/* 
 * File:   Cfg_item.hpp
 * Author: lefty
 *
 * Created on 23. prosinec 2013, 15:59
 */

#ifndef CFG_ITEM_HPP
#define	CFG_ITEM_HPP

#include <string>
#include <vector>

#include "Module_manager.hpp"
#include "Module.hpp"

/**
 * @brief Base class for configuration items such as action or location
 * 
 * Such item may look like "variable = value" in config file.
 */
class Cfg_item {
public:
    Cfg_item(std::string& line);
    std::string& get_name(void);
    virtual ~Cfg_item();
    
    static Module_manager *module_manager;
protected:
    std::string name;
    std::string value;
};

/**
 * @brief Class for configuration variable items.
 * 
 * Such item may look like "variable = module.argument=constant".
 * This variable is true if the argument obtained from module contains constant
 * in its comma-separated value.
 */
class Cfg_variable : public Cfg_item {
public:
    Cfg_variable(std::string& line);
    bool get_value(void); //!< Get boolean value of variable by querying module
    void set_value(void); //!< Set value of module argument to constant from cfg
    void reset_value(void); //!< Reset value of module argument to original value
protected:
    Module *module;
    std::string module_argument;
    std::string module_value;
};

/**
 * @brief Class for configuration location items.
 * 
 * Such item may look like "location_name = (sense1 & sense2) | !sense3"
 */
class Cfg_location : public Cfg_item {
public:
    Cfg_location(std::string& line);
    bool here(void); //!< Evaluate the term and returns if device is at location
private:
    bool eval_variable(std::string &var_name);
    bool was_here;
};

/**
 * @brief Class for configuration rule items.
 * 
 * Rule item may look like  "location_name = action1,action2,...,actionN"
 * Actions execute, when device gets into location after not being there.
 */
class Cfg_rule : public Cfg_item {
public:
    Cfg_rule(std::string& line);
    void commit(void); //!< Execute associated actions
    void rollback(void); //!< Rollback associated actions
private:
    std::vector<Cfg_variable*> dependencies;
};


#endif	/* CFG_ITEM_HPP */

