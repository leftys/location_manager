/* 
 * File:   Module.hpp
 * Author: lefty
 *
 * Created on 25. prosinec 2013, 12:52
 */

#ifndef MODULE_HPP
#define	MODULE_HPP

#include <string>
#include <thread>
#include <map>
#include <mutex>
#include <condition_variable>

/**
 * @brief Virtual base class for interacting with system
 */
class Module {
public:
    Module();
    virtual ~Module();
    //!< Tells the module to initialize itself
    virtual void activate(void);
    //!< Gets value of a variable
    virtual std::string get_status(const std::string& argument) = 0;
    //!< Sets value of a variable
    virtual void set(const std::string& variable, const std::string& value) = 0;
    virtual void watch(void) = 0; //!< Starts module thread monitoring changes
    virtual void watch_stop(void) = 0; //!< Stops module thread monitoring changes
    virtual void watch_wait(void) = 0; //!< Joins module thread monitoring changes
protected:
    bool active = false;
};

/**
 * @brief Module executing external command in order to interact with system
 * 
 * Base command should be in file PROGRAM_DIR/ext-modules/module_name.
 */
class External_module : public Module {
public:
    External_module(const std::string& name);
    External_module(const std::string& name, const int delay_ms);
    std::string get_status(const std::string &argument);
    void set(const std::string& variable, const std::string& value);
    void watch(void);
    void watch_stop(void);
    void watch_wait(void);
    static std::pair<std::string,int> exec(const std::string& cmd);
protected:
    void thread_loop(void);
    
    std::string command; //!< command to execute
    std::thread watch_thread; //!< thread monitoring changes
    std::map< std::string,std::string > old_status; //!< value in previous monitoring query
    
    std::mutex watch_mutex; //!< mutex for cv
    std::condition_variable cv; //!< condition_variable waiting for "!running"
    bool running = false; //!< false = i should terminate monitoring thread
    int delay_ms = 5000; //!< Delay between monitoring queries
};

class Module_exception : public std::exception {
public:
    Module_exception(const std::string& name, const std::string& argument);
    virtual const char *what(void) const throw();
    std::string module_name,variable;
};

class External_module_exception : public Module_exception {
public:
    External_module_exception(const std::string& name, const std::string& argument);
    virtual const char *what(void) const throw();
};

#endif	/* MODULE_HPP */
