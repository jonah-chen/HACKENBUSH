#pragma once

#include <iostream>
#include <GLFW/glfw3.h>

#define  XPOS(IN) ( (IN.fp)[0] )
#define  YPOS(IN) ( (IN.fp)[1] )
#define   LMB(IN) ( (IN.tf)[0] )
#define   RMB(IN) ( (IN.tf)[1] )
#define   K_W(IN) ( (IN.tf)[2] )
#define   K_A(IN) ( (IN.tf)[3] )
#define   K_S(IN) ( (IN.tf)[4] )
#define   K_D(IN) ( (IN.tf)[5] )
#define K_ESC(IN) ( (IN.tf)[6] )

#define FLOATPOINT_INPUTS 2
#define TRUE_FALSE_INPUTS 7


class user_inputs
{
public:
    // default constructors suffice
    user_inputs() = default;
    user_inputs(const user_inputs&) = default;
    user_inputs& operator=(const user_inputs&) = default;


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
     * relavent keys, and mouse buttons presses.
     */
    static user_inputs fetch(GLFWwindow* window);


    friend std::ostream& operator<<(std::ostream& os, const user_inputs& ui);

public:
    double fp[FLOATPOINT_INPUTS];
    bool   tf[TRUE_FALSE_INPUTS];
};
