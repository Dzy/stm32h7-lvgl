/**
 * @file lv_port_indev_templ.c
 *
 */

 /*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"

#include "usb_host.h"

#include "usbh_hid.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_parser.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void mouse_init(void);
static bool mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool mouse_is_pressed(void);
static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_mouse;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * Mouse
     * -----------------*/

    /*Initialize your touchpad if you have*/
    mouse_init();

    /*Register a mouse input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;
    indev_mouse = lv_indev_drv_register(&indev_drv);

    /*Set cursor. For simplicity set a HOME symbol now.*/
    lv_obj_t * mouse_cursor = lv_img_create(lv_disp_get_scr_act(NULL), NULL);
    lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
    lv_indev_set_cursor(indev_mouse, mouse_cursor);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Mouse
 * -----------------*/

/*Initialize your mouse*/
static void mouse_init(void)
{
    /*Your code comes here*/
}

int32_t MouseX, MouseY;
uint8_t MouseB[3];

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
  if(USBH_HID_GetDeviceType(phost) == HID_MOUSE)
  {
    HID_MOUSE_Info_TypeDef *Mouse_Info;
    Mouse_Info = USBH_HID_GetMouseInfo(phost);  // Get the info
    MouseX += (int8_t)Mouse_Info->x;  // get the x value
    MouseY += (int8_t)Mouse_Info->y;  // get the y value

    if(MouseX<0) MouseX = 0;
    if(MouseY<0) MouseY = 0;
    if(MouseX>800) MouseX = 799;
    if(MouseY>600) MouseY = 599;

    MouseB[0] = Mouse_Info->buttons[0];
    MouseB[1] = Mouse_Info->buttons[1];
    MouseB[2] = Mouse_Info->buttons[2];
  }
}

/*Will be called by the library to read the mouse*/
static bool mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    MX_USB_HOST_Process();

    /*Get the current x and y coordinates*/
    mouse_get_xy(&data->point.x, &data->point.y);

    /*Get whether the mouse button is pressed or released*/
    if(mouse_is_pressed()) {
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}

/*Return true is the mouse button is pressed*/
static bool mouse_is_pressed(void)
{
    /*Your code comes here*/
    if(MouseB[0]) {
        return true;
    }
    return false;
}

/*Get the x and y coordinates if the mouse is pressed*/
static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/

    (*x) = MouseX;
    (*y) = MouseY;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
