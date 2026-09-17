#include "x11_window.h"
#include "core/window/window.h"
#include "debug.h"

#include "egl.h"

#include <sys/mman.h>
#include <xkbcommon/xkbcommon.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/Xlib-xcb.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <thread>

#define MOUSE_BUTTON_LEFT   1
#define MOUSE_BUTTON_MIDDLE 2
#define MOUSE_BUTTON_RIGHT  3
#define SCROLL_WHEEL_UP     4
#define SCROLL_WHEEL_DOWN   5
#define XBUTTON0            8
#define XBUTTON1            9

typedef struct RendererState
{
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;
    EGLContext eglContext;
} RendererState;

namespace idrs
{
    X11_Window::X11_Window() : 
        m_display(nullptr),
        m_connection(nullptr),
        m_screen(nullptr),
        m_rndState(nullptr),
        dev(nullptr),
        evdev_result(0)
    {
        m_rndState = new RendererState{};
    }

    bool X11_Window::create(Window *wnd, const std::string &title, u32 width, u32 height, u32 style)
    {
        m_wnd = wnd;

        m_display = XOpenDisplay(nullptr);
        XAutoRepeatOff(m_display);

        m_connection = XGetXCBConnection(m_display);
        if (xcb_connection_has_error(m_connection))
        {
            printf("xcb connection failure!\n");
            return false;
        }

        XSetEventQueueOwner(m_display, XCBOwnsEventQueue); 

        const struct xcb_setup_t *setup = xcb_get_setup(m_connection);
        xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);
        s32 screen_p = 0;
        for (s32 s = screen_p; s > 0; s--)
        {
            xcb_screen_next(&it);
        }

        m_screen = it.data;
        m_window = xcb_generate_id(m_connection);

        u32 eventMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        u32 eventValues = 
            XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | 
            XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | 
            XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION | 
            XCB_EVENT_MASK_STRUCTURE_NOTIFY;

        u32 vlist[] = { m_screen->black_pixel, eventValues };
        
        xcb_void_cookie_t window = xcb_create_window(
            m_connection,
            XCB_COPY_FROM_PARENT,
            m_window,
            m_screen->root,
            0, 0,
            width, height,
            0,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            m_screen->root_visual,
            eventMask,
            vlist 
        );

        setTitle(title);
       
        wm_deleteWindow = xcbInternAtom(*m_connection, "WM_DELETE_WINDOW");
        xcb_atom_t proto = xcbInternAtom(*m_connection, "WM_PROTOCOLS");
        xcb_change_property(
            m_connection, 
            XCB_PROP_MODE_REPLACE, 
            m_window, proto, 
            4, 32, 1, 
            &wm_deleteWindow
        );

        xcb_map_window(m_connection, m_window);
        xcb_flush(m_connection);

        eglSetup();


        //
        s32 fd = open("/dev/input/by-id/usb-Microsoft_Controller_7EED8030908D-event-joystick", O_RDONLY | O_NONBLOCK);
        if (fd == -1)
        {
            return false;
        }
        evdev_result = libevdev_new_from_fd(fd, &dev);
        printf("Device Name: %s\n", libevdev_get_name(dev));
        printf("Device Bus: %#x\n", libevdev_get_id_bustype(dev));
        printf("Device Vendor ID: %#x\n", libevdev_get_id_vendor(dev));
        printf("Device Product ID: %#x\n", libevdev_get_id_product(dev));
        //

        return true;
    }

    void X11_Window::cleanup()
    {
        XAutoRepeatOn(m_display);
        xcb_destroy_window(m_connection, m_window);
    }

    void X11_Window::swapBuffers()
    {
        eglSwapBuffers(m_rndState->eglDisplay, m_rndState->eglSurface);
    }

    void X11_Window::sleep(const uint64_t duration)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    }

    void X11_Window::swapInterval(uint8_t interval)
    {
        eglSwapInterval(m_rndState->eglDisplay, interval);
    }

    void X11_Window::setTitle(const std::string &title)
    {
        xcb_change_property(
            m_connection, 
            XCB_PROP_MODE_REPLACE, 
            m_window, 
            XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 
            8, strlen(title.c_str()), title.c_str()
        );
    }

    void X11_Window::setFullscreen(bool fullscreen)
    {
        xcb_atom_t wmState = xcbInternAtom(*m_connection, "_NET_WM_STATE");
        xcb_atom_t wmFullscreen = xcbInternAtom(*m_connection, "_NET_WM_STATE_FULLSCREEN");
        xcb_client_message_event_t event;
        event.response_type = XCB_CLIENT_MESSAGE;
        event.format = 32;
        event.sequence = 0;
        event.window = m_window;
        event.type = wmState;
        event.data.data32[0] = (uint16_t)fullscreen;
        event.data.data32[1] = wmFullscreen;
        event.data.data32[2] = 0;
        event.data.data32[3] = 1;
        event.data.data32[4] = 0;

        xcb_send_event(
            m_connection, 
            0, m_window, 
            XCB_EVENT_MASK_STRUCTURE_NOTIFY, (const char*)&event
        );
        xcb_flush(m_connection);
    }

    void X11_Window::setPos(int32_t x, int32_t y)
    {
        u32 mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
        s32 values[] = { x, y };
        xcb_configure_window(m_connection, m_window, mask, values);
        xcb_flush(m_connection);
    }
    
    void X11_Window::setSize(uint32_t width, uint32_t height)
    {
        u32 mask = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
        u32 values[] = { width, height };
        xcb_configure_window(m_connection, m_window, mask, values);
        xcb_flush(m_connection);
    }

    void X11_Window::setFlash(uint32_t count, unsigned long timeout, bool sound){}
    void X11_Window::getCursorPos(int32_t &outX, int32_t &outY, bool relative)
    {
        xcb_query_pointer_cookie_t pcookie = xcb_query_pointer(m_connection, m_window);
        xcb_query_pointer_reply_t *preply = xcb_query_pointer_reply(m_connection, pcookie, nullptr);

        /* NOTE: Invert the cursor's Y position because 
         *       XCB has the origin in the top-left,
         *       while OpenGL is the bottom-left
         */
        if (relative)
        {
            outX = preply->win_x;
            outY = m_wnd->m_height - preply->win_y;
        }
        else
        {
            outX = preply->root_x;
            outY = m_wnd->m_height - preply->root_y;
        }

        delete(preply);
    }

    void X11_Window::getPos(int32_t &outX, int32_t &outY)
    {
        xcb_translate_coordinates_cookie_t cookie = xcb_translate_coordinates(m_connection, m_window, m_screen->root, 0, 0);
        xcb_translate_coordinates_reply_t *reply = xcb_translate_coordinates_reply(m_connection, cookie, nullptr);
        outX = reply->dst_x;
        outY = reply->dst_y;

        delete(reply);
    }

    void X11_Window::getSize(uint32_t &outWidth, uint32_t &outHeight)
    {
        xcb_get_geometry_reply_t *reply = xcb_get_geometry_reply(m_connection, xcb_get_geometry(m_connection, m_window), nullptr);
        outWidth  = reply->width;
        outHeight = reply->height;

        delete(reply);
    }

    const bool X11_Window::isFocused(){return true;}
    const uint32_t X11_Window::nativeStyle(const uint32_t style){return 0;}

    xcb_atom_t X11_Window::xcbInternAtom(xcb_connection_t &conn, const std::string &atomName)
    {
        xcb_intern_atom_cookie_t wmCookie = xcb_intern_atom(&conn, 0, atomName.length(), atomName.c_str());
        xcb_intern_atom_reply_t *wmReply = xcb_intern_atom_reply(m_connection, wmCookie, nullptr);
        xcb_atom_t atom = wmReply->atom;

        delete(wmReply);
        return atom;
    }

    void X11_Window::pollMessages()
    {
        struct input_event ev;
        evdev_result = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        Event e;
        if (evdev_result == LIBEVDEV_READ_STATUS_SUCCESS)
        {
            if (ev.type != 0)
            {
                input = ev;
                e.type = Event::Type::_DeviceInput;
                e._inputDev.data = (u8*)&input;
                e._inputDev.eventQueue = (void*)&m_wnd->m_events;
                printf("Before: %p\n", e._inputDev.data);
            }
        }
        if (e.type != Event::Type::Default)
        {
            m_wnd->m_events.push(e);
        }

        xcb_generic_event_t *event;
        while ((event = xcb_poll_for_event(m_connection)))
        {
            if (!event)
            {
                break;
            }

            Event e;
            switch (event->response_type & ~0x80)
            {
                case XCB_CLIENT_MESSAGE:
                {
                    xcb_client_message_event_t *client = (xcb_client_message_event_t *)event;
                    if (client->data.data32[0] == wm_deleteWindow)
                    {
                        m_wnd->close();
                    }
                } break;
                
                case XCB_CONFIGURE_NOTIFY:
                {
                    xcb_configure_notify_event_t *cfg = (xcb_configure_notify_event_t *)event;

                    e.type = Event::Type::WindowResize;
                    e.wndResizeInfo.width  = cfg->width;
                    e.wndResizeInfo.height = cfg->height;
                } break;

                case XCB_KEY_PRESS:
                {
                    xcb_key_press_event_t *kpe = (xcb_key_press_event_t *)event;
                    xcb_keycode_t code = kpe->detail;
                    KeySym key = XkbKeycodeToKeysym(m_display, (KeyCode)code, 0, 1);

                    e.type = Event::Type::KeyPressed;
                    e.keyCode = (s32)key;
                } break;

                case XCB_KEY_RELEASE:
                {
                    xcb_key_release_event_t *kpe = (xcb_key_release_event_t *)event;
                    xcb_keycode_t code = kpe->detail;
                    KeySym key = XkbKeycodeToKeysym(m_display, (KeyCode)code, 0, 1);

                    e.type = Event::Type::KeyReleased;
                    e.keyCode = (s32)key;
                } break;

                case XCB_BUTTON_PRESS:
                {
                    xcb_button_press_event_t *bpe = (xcb_button_press_event_t *)event;

                    switch (bpe->detail)
                    {
                        case SCROLL_WHEEL_UP:
                        {
                            e.type = Event::Type::MouseScroll;
                            e.scrollDelta = 1;
                        } break;

                        case SCROLL_WHEEL_DOWN:
                        {
                            e.type = Event::Type::MouseScroll;
                            e.scrollDelta = -1;
                        } break;

                        default:
                        {
                            e.type = Event::Type::MousePressed;
                            e.mouseButton = bpe->detail;
                        } break;
                    }
                } break;

                case XCB_BUTTON_RELEASE:
                {
                    xcb_button_release_event_t *bre = (xcb_button_release_event_t *)event;

                    e.type = Event::Type::MouseReleased;
                    e.mouseButton = bre->detail;
                } break;

                case XCB_MOTION_NOTIFY:
                {
                    static s32 lastX;
                    static s32 lastY;
                    xcb_motion_notify_event_t *mme = (xcb_motion_notify_event_t *)event;

                    e.type = Event::Type::MouseMove;
                    e.mousePos.x = mme->event_x - lastX;
                    e.mousePos.y = mme->event_y - lastY;
                    lastX = mme->event_x;
                    lastY = mme->event_y;
                } break;

                default: break;
            }

            if (e.type != Event::Type::Default)
            {
                m_wnd->m_events.push(e);
            }

            delete(event);
        }
    }

    void X11_Window::eglSetup()
    {
        bool result = gladLoaderLoadEGL(EGL_NO_DISPLAY);
        ASSERT(result, "GLAD failed to load EGL 1.0 function pointers!");

        m_rndState->eglDisplay = eglGetDisplay((EGLNativeDisplayType)m_display);
        result = eglInitialize(m_rndState->eglDisplay, nullptr, nullptr);
        ASSERT(result, "EGL failed to initialize!");

        result = gladLoadEGL(m_rndState->eglDisplay, (GLADloadfunc)eglGetProcAddress);
        ASSERT(result, "GLAD failed to load EGL 1.5 function pointers");

        EGLAttrib displayAttrs[] = 
        {
            EGL_PLATFORM_X11_SCREEN_EXT, 0, EGL_NONE
        };
        m_rndState->eglDisplay = eglGetPlatformDisplay(EGL_PLATFORM_X11_EXT, m_display, displayAttrs);
        result = eglBindAPI(EGL_OPENGL_API);
        ASSERT(result, "Failed to find API");

        EGLint configAttrs[] = 
        {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
            EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
            EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 24, EGL_STENCIL_SIZE, 8,
            EGL_NONE
        };
        EGLConfig config;
        EGLint cfgCount;
        result = eglChooseConfig(m_rndState->eglDisplay, configAttrs, &config, 1, &cfgCount);
        ASSERT(result, "Failed to choose config!");

        EGLAttrib surfaceAttrs[] = 
        {
            EGL_GL_COLORSPACE, EGL_GL_COLORSPACE_LINEAR,
            EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
            EGL_NONE
        };
        EGLint contextAttrs[] = 
        {
            EGL_CONTEXT_MAJOR_VERSION, 4,
            EGL_CONTEXT_MINOR_VERSION, 4,
            EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
            EGL_NONE
        };

        m_rndState->eglContext = eglCreateContext(m_rndState->eglDisplay, config, EGL_NO_CONTEXT, contextAttrs);
        m_rndState->eglSurface = eglCreatePlatformWindowSurface(m_rndState->eglDisplay, config, &m_window, surfaceAttrs);
        result = eglMakeCurrent(
            m_rndState->eglDisplay, 
            m_rndState->eglSurface, 
            m_rndState->eglSurface, 
            m_rndState->eglContext);

        ASSERT(result, "Failed to make context current!");
    }
}