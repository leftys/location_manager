/* 
 * File:   Module_monitor.cpp
 * Author: lefty
 * 
 * Created on 19. březen 2014, 14:11
 */

#include <glog/logging.h>
#include <functional>
#include <iomanip>
#include <iostream>
#include <fstream>
extern "C" { 
#include <sys/poll.h>
}
#include <X11/extensions/Xrandr.h>
#include <X11/Xatom.h>
#include <gtk-3.0/gdk/gdkx.h>
#include <gtk-3.0/gtk/gtk.h>

#include "main.hpp"
#include "Module_monitor.hpp"
#include "Module.hpp"
#include "Configuration.hpp"

using namespace std;

Module_monitor::Module_monitor() : Module() {
    inotify_fd = inotify_init();
}
Module_monitor::~Module_monitor() {}

std::string Module_monitor::get_status(const std::string &argument) {
    if (argument != "id") {
        throw Module_exception("monitor",argument);
    }
    ifstream f_status (vga_status_file);
    string status;
    f_status >> status;
    if (status == "connected") 
        return get_vga_name()+",Any";
    else 
        return "None";
}

void Module_monitor::set(const std::string &variable, const std::string &value) {
    throw Module_exception("monitor",variable);
}


void Module_monitor::watch(void) {
    //Set-up hotplug callback
    watch_fd = inotify_add_watch(inotify_fd, vga_status_file.c_str(), IN_CLOSE_WRITE | IN_MODIFY);
    if (watch_fd == -1) {
        LOG(FATAL) << "Error creating an inotify monitor watch";
    }
    stop_thread = 0;
    change = false;
    watch_thread = thread(std::bind(&Module_monitor::thread_loop,this));
    LOG(INFO) << "Watching VGA connection";
}

void Module_monitor::watch_stop(void) {
    stop_thread = 1;
}

void Module_monitor::watch_wait(void) {
    watch_thread.join();
}

void Module_monitor::thread_loop() {
    pollfd pfd = { inotify_fd, POLLIN, 0 };
    do {
        char buf[sizeof(struct inotify_event *)];
        //int ret = poll(&pfd, 1, 1000); //poll 1fd, 1000ms timeout
        int ret = read(inotify_fd, buf, sizeof(buf));
        LOG_ASSERT(ret >= 0);
        string new_status = get_status("id");
        if (ret > 0 || (old_status != new_status)) {
            LOG(INFO) << "VGA status changed!";
            Configuration::reprocess();
            change = false;
        }
        old_status = new_status;
    } while (!stop_thread);
    //LOG(INFO) << "Thread " << this_thread::get_id() << " ended.";
}

static guint8 read_edid_data (Display *xdisplay, RROutput output) {
    unsigned char *prop;
    int            actual_format;
    unsigned long  nitems, bytes_after;
    Atom           actual_type;
    Atom           edid_atom;
    guint8        *result = NULL;

    edid_atom = gdk_x11_get_xatom_by_name (RR_PROPERTY_RANDR_EDID);

    if (edid_atom != None)
    {
        if (XRRGetOutputProperty (xdisplay, output, edid_atom, 0, 100,
                                  False, False, AnyPropertyType,
                                  &actual_type, &actual_format, &nitems,
                                  &bytes_after, &prop) == Success)
        {
            if (actual_type == XA_INTEGER && actual_format == 8)
                result = static_cast<guint8 *>(g_memdup (prop, nitems));
        }

        XFree (prop);
    }

    return *result;
}

string Module_monitor::get_vga_name() {
    string name;
    int retcode;
    tie(name, retcode) = External_module::exec(dirname + "/helpers/get_vga_name");
    LOG_ASSERT(retcode == 0);
    return std::move(name);
}

/*string Module_monitor::get_vga_name() {
    int argc=1;
    char *arg0= (char *)"profile_manager";
    char **argv = (char **)malloc(sizeof(char *));
    argv[0] = arg0;
    gtk_init(&argc,&argv);
    
    GdkWindow *root_window = gdk_get_default_root_window ();
    GdkDisplay *display = gdk_display_get_default();
    Display *xdisplay = gdk_x11_display_get_xdisplay(display);
    XRRScreenResources *res = XRRGetScreenResources(xdisplay, GDK_WINDOW_XID (root_window));
    XRROutputInfo **out_info = g_new0(XRROutputInfo*, res->noutput);
    for (int n=0; n<res->noutput; n++) {
        out_info[n] = XRRGetOutputInfo (xdisplay, res, res->outputs[n]);
        guint8 edid = read_edid_data(xdisplay, res->outputs[n]);
        cout << "Name: " << out_info[n]->name << " " << edid << ";" << endl;
        XRRFreeOutputInfo(out_info[n]);
    }
    XRRFreeScreenResources (res);
}*/