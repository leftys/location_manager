/* 
 * File:   Module_usb.cpp
 * Author: lefty
 * 
 * Created on 19. březen 2014, 14:11
 */

#include <glog/logging.h>
#include <set>
#include <functional>
#include <iomanip>
#include "Module_usb.hpp"
#include "Configuration.hpp"

using namespace std;

// Static members init:
unordered_set<string> Module_usb::ids;
std::atomic_bool Module_usb::change;
unsigned char Module_usb::module_usb_instances = 0;

Module_usb::Module_usb() : Module() {
    libusb_init(NULL);
    LOG_ASSERT(++module_usb_instances == 1); // More than 1 instance of this class is not allowed!
    if (libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG) == 0)
        LOG(ERROR) << "Cannot watch USB hotplug! libusb-1.0 lacks capability to do so.";
}
Module_usb::~Module_usb() {
    libusb_exit(NULL);
}

std::string Module_usb::get_status(const std::string &argument) {
    if (argument != "id") {
        throw Module_exception("monitor",argument);
    }
    ostringstream oss;
    for (string id : ids) {
        oss << id << ",";
    }
    string ret = oss.str();
    if (!ids.empty())
        ret.pop_back(); // remove trailing ','
    return std::move(ret);
}

void Module_usb::set(const std::string &variable, const std::string &value) {
    throw Module_exception("monitor",variable);
}

int Module_usb::usb_hotplug_callback_tmp(struct libusb_context *ctx, struct libusb_device *dev, libusb_hotplug_event event, void *user_data) {
    struct libusb_device_descriptor desc;
    libusb_get_device_descriptor(dev, &desc);
    
    ostringstream oss;
    oss << setw(4) << setfill('0') << hex << desc.idVendor ;
    oss << ":";
    oss << setw(4) << setfill('0') << hex << desc.idProduct;
    string id = oss.str();
            
    if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
        VLOG(1) << "Connected " << id;
        ids.insert(id);
    } else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT) {
        VLOG(1) << "Disconnected " << id;
        ids.erase(id);
    } else {
        LOG(WARNING) << "Unhandled USB event " << event;
    }
    change = true;
    return 0;
}

void Module_usb::watch(void) {
    /*typedef int function_t(struct libusb_context*, struct libusb_device*, libusb_hotplug_event, void*);
    std::function<function_t> 
        fcn = std::bind(&Module_usb::usb_hotplug_callback,this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
            );*/
    //libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_DEBUG);

    //Set-up usb hotplug callback
    int rc = libusb_hotplug_register_callback(NULL,  
            static_cast<libusb_hotplug_event>(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
            LIBUSB_HOTPLUG_ENUMERATE, 
            LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, 
            //fcn.target<function_t>(),
            usb_hotplug_callback_tmp,
            NULL, &handle);
    if (rc != LIBUSB_SUCCESS) {
        LOG(FATAL) << "Error creating an USB hotplug callback";
    }
    stop_thread = 0;
    change = false;
    watch_thread = thread(std::bind(&Module_usb::thread_loop,this));
    LOG(INFO) << "Watching USBs";
}

void Module_usb::watch_stop(void) {
    libusb_hotplug_deregister_callback(NULL,handle);
    stop_thread = 1;
}

void Module_usb::watch_wait(void) {
    watch_thread.join();
}

void Module_usb::thread_loop() {
    static timeval time = {60,0};
    do {
        libusb_handle_events_timeout_completed(NULL, &time, &stop_thread);
        if (change) {
            Configuration::reprocess();
            change = false;
        }
    } while (!stop_thread);
    //LOG(INFO) << "Thread " << this_thread::get_id() << " ended.";
}
