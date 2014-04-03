/* 
 * File:   Module_usb.hpp
 * Author: lefty
 *
 * Created on 19. březen 2014, 14:11
 */

#ifndef MODULE_USB_HPP
#define	MODULE_USB_HPP

#include <string>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <atomic>

#include "Module.hpp"
#include <libusb-1.0/libusb.h>

class Module_usb : public Module {
public:
    Module_usb();
    ;;
    virtual ~Module_usb();
    virtual std::string get_status(const std::string& argument);
    virtual void set(const std::string& variable, const std::string& value);
    virtual void watch(void); 
    virtual void watch_stop(void); 
    virtual void watch_wait(void);
    
private:
    static int usb_hotplug_callback_tmp(struct libusb_context *ctx, struct libusb_device *dev,
        libusb_hotplug_event event, void *user_data);
    void thread_loop();
    
    libusb_hotplug_callback_handle handle;
    static std::unordered_set<std::string> ids;
    
    int stop_thread;
    static std::atomic_bool change;
    std::thread watch_thread;
    static unsigned char module_usb_instances;
};

#endif	/* MODULE_USB_HPP */
