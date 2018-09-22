#ifndef __vbe_h__
#define __vbe_h__

#include <typedef.h>


typedef struct vbe_infos
{
    uint8_t asig [4];
    uint16_t version;
    char reserved0[8];
    uint32_t video_list_mode;
    uint16_t memory_size;
    char reservend1[512 - 20];
}__attribute__ ((packed)) vbe_infos_t;


// Suporte a VBE3.0
typedef struct vbe_info_mode {

    // VBE
    uint16_t mode_attr;
    uint8_t  window_a_attr;
    uint8_t  window_b_attr;
    uint16_t window_gran;
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;

    // VBE1.2
    uint16_t x_resolution; //width;
    uint16_t y_resolution; //hight;
    uint8_t  x_char_size;
    uint8_t  y_char_size;
    uint8_t  planes;
    uint8_t  bpp;
    uint8_t  number_of_banks;
    uint8_t  memory_model;
    uint8_t  bank_size;
    uint8_t  number_of_image_page;
    uint8_t  reserved0;
    uint8_t  red_mask_size;
    uint8_t  red_field_position;
    uint8_t  green_mask_size;
    uint8_t  green_field_position;
    uint8_t  blue_mask_size;
    uint8_t  blue_field_position;
    uint8_t  reservend_mask_size;
    uint8_t  reservend_field_position;
    uint8_t  direct_color_info;

    // VBE2.0
    uint32_t framebuffer;
    uint32_t reserved1;
    uint16_t reserved2;

    // VBE3.0
    uint16_t byte_scan_line;
    uint8_t  bnk_number_of_image_pages;
    uint8_t  line_number_of_image_pages;
    uint8_t  line_red_mask_size;
    uint8_t  line_red_field_position;
    uint8_t  line_green_mask_size;
    uint8_t  line_green_field_position;
    uint8_t  line_blue_mask_size;
    uint8_t  line_blue_field_position;
    uint8_t  line_reservend_mask_size;
    uint8_t  line_reservend_field_position;
    uint32_t max_pixel_clock;
  
    char dup[189+1];

}__attribute__ ((packed)) vbe_info_mode_t;

#endif
