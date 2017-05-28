#pragma once

#include "glm.h"
#include "gl.h"

namespace camera {
  // TODO: should be rewritten with a CameraMode implementation per mode that does the translation input -> camera action
  enum Mode {
    CENTERED,
    FREE_FLIGHT,
  };

  static const std::string modes_names[] = {"CENTERED", "FREE_FLIGHT"};

  class Camera {
  public:
    Camera();
    Mode mode;
    glm::vec3 pos;
    glm::vec3 direction;
    glm::vec3 up;

    GLfloat base_speed() const;

    void offset_speed(GLfloat speed_offset);
    void offset_pitch(GLfloat pitch_offset);
    void offset_yaw(GLfloat yaw_offset);
    void update_direction_from_angles();

  private:
    const GLfloat base_max_speed_ = 100.f;
    const GLfloat base_min_speed_ = 0.01f;
    GLfloat pitch_ = 0.0f;
    GLfloat yaw_ = -90.0f;
    GLfloat base_speed_ = 2.5f;
  };
}
