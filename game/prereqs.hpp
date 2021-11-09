/**
 * @file prereqs.hpp
 * @author Jonah Chen
 * @brief Defines the strict prerequisites for the hackenbush game. Includes:
 * - Branches
 * - 
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#pragma once

#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <glm/glm.hpp>

namespace game
{
    // Note that this doesn't work, because it can only deal with finitely many
    // branches.
    //
    // class branch
    // {
    // public:
    //     /**
    //      * @brief Construct a new branch object
    //      * 
    //      * @param type 
    //      * @param position 
    //      */
    //     branch(branch_type type);
    //
    //     /**
    //      * @brief Get the type of the branch
    //      * 
    //      * @return branch_type 
    //      */
    //     branch_type get_type() const;
    //
    // private:
    //     branch_type         type_;
    //     glm::vec3           position1_;
    //     glm::vec3           position2_;
    //     // note that this isn't good enough to store the potentially infinite 
    //     // number of children a branch has
    //     std::set<branch*>   children_;
    // };

    /**
     * @brief the branch class describes the any type of branch that exists in 
     * this implementation of hackenbush. Instances of this class are invalid 
     * branches, which describes the ground.
     * 
     * @details branches must implement the following functions:
     * - the call operator
     * - the render method
     * - the log method
     * 
     * The default implementation of the call operator calls the children if 
     * they exist, and an empty set otherwise. This functionality should be 
     * implemented in all derived classes.
     * 
     * The default implementation of the other methods does what is logical,
     * which is close to nothing.
     */
    class branch
    {
    public:
        /**
         * @brief types of branches in the game
         * - red:   1
         * - green: 0
         * - blue: -1
         */
        enum branch_type : int8_t
        {
            invalid = -127,
            blue    =    1,
            green   =    0,
            red     =   -1
        };


        /**
         * @brief Construct a new branch object with invalid type and no 
         * children
         * 
         */
        branch() : type_(invalid), children_(nullptr) {}

        
        /**
         * @brief Construct a new branch object with the invalid type and with 
         * children
         * 
         * @param children pointer to the children branch.
         * 
         */        
        branch(branch* children) : type_(invalid), children_(children) {}


        /**
         * @brief Get the branches that is contained in the volume specified
         * by two diagonally opposite corners.
         * 
         * @param topleft: 3d position of the top left corner of the volume. 
         * Note that the top left corner is the corner with the smallest x, y,
         * and z values.
         * @param bottomright: 3d position the bottom right corner of the 
         * volume. Note that the bottom right corner is the corner with the
         * largest x, y, and z values.
         * 
         * @return an unordered_set of branch references that contains all the 
         * branches contained in the volume which can be used to render branches
         * to the screen and interact with the player.
         * 
         */
        virtual std::unordered_set<branch*> 
        operator() (const glm::vec3 &topleft, const glm::vec3 &bottomright);


        /**
         * @brief modify the parameters to the two vertex position of this and
         * only this branch. Note that these are in no particular order.
         * 
         * @param vertex1 3d vector reference to store the first vertex 
         * position.
         * @param vertex2 3d vector reference to store the second vertex 
         * position
         * 
         */
        virtual void render (glm::vec3 &vertex1, glm::vec3 &vertex2) const;


        /**
         * @brief write relevant logging info to the output stream.
         * 
         * @param os reference to output stream. Defaults to std::cout.
         */
        virtual void log(std::ostream &os = std::cout) const;

        /**
         * @brief Get the type of the branch.
         * 
         * @return branch_type 
         */
        inline branch_type get_type() const { return type_; }

    protected:
        branch_type type_;      // type of the branch 
        branch*     children_;  // children of the branch
    };

    namespace branches
    {
        /**
         * @brief the single class describes a single branch which may have
         * up to infinitely many children. Note that this class does not have
         * a default constructor.
         * 
         */
        class single : protected branch
        {
        public:
            /**
             * @brief Construct a new single branch.
             * 
             * @param pos1 the position of the first vertex of the branch.
             * @param pos2 the position of the second vertex of the branch.
             * @param type the type of the branch.
             */
            single(const glm::vec3& pos1, const glm::vec3& pos2, 
                   branch_type type);

            single (const single&) = delete;
            single& operator= (const single&) = delete;

            /**
             * @brief Get the branches that is contained in the volume specified
             * by two diagonally opposite corners.
             * 
             * @param topleft: 3d position of the top left corner of the volume. 
             * Note that the top left corner is the corner with the smallest x, 
             * y, and z values.
             * @param bottomright: 3d position the bottom right corner of the 
             * volume. Note that the bottom right corner is the corner with the
             * largest x, y, and z values.
             * 
             * @return an unordered_set of branch references that contains all 
             * the branches contained in the volume which can be used to render
             * branches to the screen and interact with the player.
             * 
             */
            std::unordered_set<branch*> 
            operator() (const glm::vec3 &topleft, 
                        const glm::vec3 &bottomright) override;

            
            /**
             * @brief modify the parameters to the two vertex position of this
             * and only this branch. Note that these are in no particular order.
             * 
             * @param vertex1 3d vector reference to store pos1_.
             * @param vertex2 3d vector reference to store pos2_.
             * 
             */
            void render (glm::vec3 &vertex1, glm::vec3 &vertex2) const override;
        private:
            glm::vec3 pos1_;
            glm::vec3 pos2_;
        };

        /**
         * @brief the finite class describes finitely many branches which may
         * each have up to infinitely many children. Note that this class does
         * 
         */
        class finite : protected branch
        {
        public:
            
        };
    }
}