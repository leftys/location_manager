/* 
 * File:   Configuration.hpp
 * Author: lefty
 *
 * Created on 23. prosinec 2013, 15:38
 */

#ifndef CONFIGURATION_HPP
#define	CONFIGURATION_HPP

#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <functional>
#include <exception>

#include "Cfg_item.hpp"

class Configuration {
public:
    Configuration(std::string cfg_file_name);
    //Configuration(const Configuration& orig);
    //virtual ~Configuration();
    static void reprocess(void);
    
    static std::map<std::string,Cfg_variable> senses,actions;
    static std::map<std::string,Cfg_location> locations;
    static std::map<std::string,Cfg_rule> rules;
private:
    void handle_line(std::string& line);
    
    static std::mutex reprocess_mutex;
    std::string section;
    std::function<void(std::string&)> section_method;
};

class Referenced_element_missing : public std::exception {
public:
    Referenced_element_missing(const std::string& message);
    virtual const char *what(void) const throw();
    std::string message;
};

#endif	/* CONFIGURATION_HPP */

