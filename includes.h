/*
 * =====================================================================================
 *
 *       Filename:  includes.h
 *
 *    Description:  this file holds some includes, defines and using statements
 *
 *        Created:  04/14/2020 11:48:42 PM
 *       Compiler:  g++
 *
 *         Author:  Jon Baker 
 *
 * =====================================================================================
 */

#include <SDL2/SDL.h> //we're just using the base SDL2 libraries - not going to get into the extensions right now
#include <cstdlib>
#include <random>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

//vector math library GLM
//usage details here: https://glm.g-truc.net/0.9.9/index.html
#define GLM_FORCE_SWIZZLE //this is for accessing vector elements with vec.x, etc
#define GLM_SWIZZLE_XYZW  //swizzling specifically refers to all combinations, e.g. .xyz, .xyy, .zzz, etc for constructing other vectors
#include "glm/glm.hpp" //general vector types
#include "glm/ext.hpp" //string cast
#include "glm/gtx/string_cast.hpp"  //also string cast, I guess
#include "glm/gtc/matrix_transform.hpp" // for glm::ortho
#include "glm/gtc/type_ptr.hpp" //to send matricies gpu-side
#include "glm/gtx/transform.hpp"  //rotations, scaling, etc
