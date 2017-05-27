#pragma once

#include "glm.h"
#include "Drawable.h"

class Renderer {
public:
    /**
     * Render a Drawable on the scene, using the given world tranform (usually camera).
     * The Drawable's shader must have a mat4 "transform" entry point.
     * @param drawable
     * @param transfom
     */
    void render(const Drawable& drawable, const glm::mat4& transfom) const;

private:
};

