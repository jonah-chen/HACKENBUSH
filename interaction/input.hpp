/**
 * @file input.hpp
 * @author Jonah Chen
 * @brief contains the necessary functions to obtain and process the user input 
 * which is used to control the camera, game, and the client itself.
 * @version 1.0
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "render/camera.hpp"
#include "game/prereqs.hpp"
#include <iostream>
#include <limits>
#include <sstream>
#include <GLFW/glfw3.h>
#include "game/game.hpp"

#define     YPOS(IN) ( ((IN).fp)[1] )
#define     XPOS(IN) ( ((IN).fp)[0] )
#define      LMB(IN) ( ((IN).tf)[0] )
#define      RMB(IN) ( ((IN).tf)[1] )
#define  K_SPACE(IN) ( ((IN).tf)[2] ) // move up
#define K_LSHIFT(IN) ( ((IN).tf)[3] ) // move down
#define      K_W(IN) ( ((IN).tf)[4] ) // move forward
#define      K_A(IN) ( ((IN).tf)[5] ) // move left
#define      K_S(IN) ( ((IN).tf)[6] ) // move down
#define      K_D(IN) ( ((IN).tf)[7] ) // move right
#define    K_ESC(IN) ( ((IN).tf)[8] ) // change to menu
#define      K_P(IN) ( ((IN).tf)[9] ) // pass your turn
#define  K_LCTRL(IN) ( ((IN).tf)[10]) // sprint

#define DOWN(KEY, CUR, PREV) (KEY(CUR) and !KEY(PREV))

#define MIN_WACK_DISTANCE 0.17f

/**
 * @brief class storing the use input data at a given frame. Members should not 
 * be accessed directly. Rather, they should be accessed using the macros 
 * defined above.
 * 
 * @details We use stack allocated arrays to store the input data. This is 
 * at least 2x faster than using individual variables for each input.
 * 
 */
class user_inputs
{
private:
	static constexpr std::size_t FLOATPOINT_INPUTS = 2;
	static constexpr std::size_t TRUE_FALSE_INPUTS = 11;
public:
	// default constructors suffice
	user_inputs() = default;

	user_inputs(const user_inputs &) = default;

	user_inputs &operator=(const user_inputs &) = default;


	/**
	 * @param rhs usually the previous inputs
	 * @return the difference between the two inputs. meaning:
	 * - the "displacement" of the cursor (divide by frame time yields velocity)
	 * - the "down event" of the buttons (true if the button was pressed down
	 *   exactly during the current frame, false otherwise)
	 */
	user_inputs operator-(const user_inputs &rhs) const;


	/**
	 * @brief Fetch the user inputs from the GLFW window and store them in the
	 * user_inputs struct.
	 *
	 * @param window GLFWwindow pointer to the window to fetch the inputs from
	 * @return struct of the user inputs consisting of the mouse position,
	 * relevant keys, and mouse buttons presses.
	 */
	static user_inputs fetch(GLFWwindow *window);

	friend std::ostream &operator<<(std::ostream &os, const user_inputs &ui);
public:
	std::array<double, FLOATPOINT_INPUTS> fp;
	std::array<bool, TRUE_FALSE_INPUTS> tf;
};

/**
 * Move the camera given the user inputs.
 *
 * @warning acceleration is not implemented.
 *
 * @param camera camera object to move.
 * @param cur_state current game state.
 * @note properties is not currently used, but will be used for acceleration.
 * @param inputs user inputs at the current frame.
 * @param last_inputs user inputs at the previous frame.
 * @param fps frames per second used for the game.
 */
void execute_movement(render::camera &camera, const game::properties &cur_state,
					  const user_inputs &inputs, const user_inputs &last_inputs,
					  float fps = 60.0f);


/**
 * @brief Find the selected edge from a collection of candidates. The selected 
 * edge is the edge that is the edge closest to the direction the player is 
 * facing and must be within a given tolerance.
 * 
 * @param camera a camera object describing the player's position and 
 * orientation.
 * @param properties a game properties object describing the current game 
 * state.
 * @param inputs user inputs at the current frame. 
 * @param candidates a collection of edges that are currently rendered on the 
 * screen.
 * @return a pointer to the selected edge.
 * @return nullptr if no edge should be selected.
 */
game::edge *select(const render::camera &camera, const game::properties
&properties, const user_inputs &inputs, const game::edge::container
				   &candidates);
