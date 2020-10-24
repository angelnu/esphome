#pragma once

struct color_rgbw_t {
    float r;
    float g;
    float b;
    float w;
};

#define color_parms(color) color.r, color.g, color.b, color.w

color_rgbw_t color_sunday    = {0.0, 0.0, 0.0, 1.0};
color_rgbw_t color_twinlight = {1.0, 0.0, 0.0, 0.0};
color_rgbw_t color_night     = {0.0, 0.0, 1.0, 0.0};
color_rgbw_t color_skyblue   = {0.0, 0.0, 1.0, 0.1};
color_rgbw_t color_lighting  = {1.0, 1.0, 1.0, 0.1};
    