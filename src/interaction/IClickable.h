//
// Created by bcub3d-laptop-dell on 12/12/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_ICLICKABLE_H
#define OPENGLPLOTLIVE_PROJ_ICLICKABLE_H

namespace GLPL {

    class IClickable {
    public:
        // Constructor
        IClickable();

        // Functions
        virtual void onLeftClick() = 0;
        void setActive(bool activeState);
        bool isActive();
        void toggleActive();

    protected:
        bool activated = false;

    };

}

#endif //OPENGLPLOTLIVE_PROJ_ICLICKABLE_H
