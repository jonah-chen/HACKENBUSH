/**
 * @file constants.hpp
 * @author Jonah Chen
 * @brief contains some common constants that are used in the whole project.
 * @version 1.0
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

// define max search depths
#define DEFAULT_MAX_DEPTH 256
#define DEFAULT_MAX_BREADTH 256

// define negative int64_t constants: `special` numbers
#define INF (int64_t)0x92345678abcdef10
#define NOT_FOUND (int64_t)0x902345678abcdef1

// define hashing constants
#define HASH_X_SEED (std::size_t)0x159a4f25e2fb1
#define HASH_Y_SEED (std::size_t)0x371607f38c6f9
#define HASH_Z_SEED (std::size_t)0xfbac09fe27a7
#define HASH_CONST  (std::size_t)0x2cf107d98dbb5

// define the rate geometric series shrinks
#define GEOMETRIC_CONSTANT 0.9f

// floating point division correction (for division by zero)
#define FLOAT_EPSILON 1e-7f

// quarternion error tolerance to correct camera roll
#define QUARTERNION_ERROR_TOLERANCE 1e-3f

// max edges and nodes to render
#define RENDER_LIMIT 4096

// RGBA colors
#define RED_CROSSHAIR_COLOR     1.0f,0.0f,0.0f,1.0f
#define BLUE_CROSSHAIR_COLOR    0.0f,0.0f,1.0f,1.0f
#define UNSELECTED_NODE_COLOR   1.0f,1.0f,1.0f,0.4f
#define SELECTED_NODE_COLOR_0   0.0f,0.0f,0.0f,0.7f
#define SELECTED_NODE_COLOR_R   0.8f,0.0f,0.0f,0.7f
#define SELECTED_NODE_COLOR_G   0.0f,0.8f,0.0f,0.7f
#define SELECTED_NODE_COLOR_B   0.0f,0.0f,0.8f,0.7f
#define GROUND_COLOR            0.2f,0.2f,0.2f,1.0f