/* 
 * File:   Module_monitor.hpp
 * Author: lefty
 *
 * Created on 19. březen 2014, 14:11
 */

#ifndef MODULE_MONITOR_HPP
#define	MODULE_MONITOR_HPP

#include <string>
#include <thread>
#include <mutex>
#include <atomic>

#include "Module.hpp"
#include <inotifytools/inotify.h>

class Module_monitor : public Module {
public:
    Module_monitor();
    virtual ~Module_monitor();
    virtual std::string get_status(const std::string& argument);
    virtual void set(const std::string& variable, const std::string& value);
    virtual void watch(void);
    virtual void watch_stop(void); 
    virtual void watch_wait(void);
    
private:
    void thread_loop();
    std::string get_vga_name();
    
    int inotify_fd, watch_fd;
    const std::string vga_status_file = "/sys/class/drm/card0-VGA-1/status";
    
    std::atomic_bool change;
    bool stop_thread;
    std::thread watch_thread;
    std::string old_status;
};

#endif	/* MODULE_MONITOR_HPP */
