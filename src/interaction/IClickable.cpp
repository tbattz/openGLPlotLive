//
// Created by bcub3d-laptop-dell on 12/12/20.
//

#include "IClickable.h"


namespace GLPL {

    IClickable::IClickable() {

    }

    void IClickable::setActive(bool activeState) {
        activated = activeState;
    }

    bool IClickable::isActive() {
        return activated;
    }

    void IClickable::toggleActive() {
        activated = !activated;
    }

}