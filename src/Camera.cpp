#include "Camera.h"

namespace camera {
  Camera::Camera()
      : mode(Mode::CENTERED)
      , pos(0.0f, 0.0f, 3.0f)
      , up(0.0f, 1.0f, 0.0f) {

  }

  void Camera::offset_speed(GLfloat speed_offset) {
    base_speed_ += speed_offset;
    if (base_speed_ < base_min_speed_) {
      base_speed_ = base_min_speed_;
    } else if (base_speed_ > base_max_speed_) {
      base_speed_ = base_max_speed_;
    }
  }

  void Camera::offset_pitch(GLfloat pitch_offset) {
    pitch_ += pitch_offset;
    if (pitch_ > 89.0f) {
      pitch_ = 89.0f;
    } else if (pitch_ < -89.0f) {
      pitch_ = -89.0f;
    }
  }

  void Camera::offset_yaw(GLfloat yaw_offset) {
    yaw_ += yaw_offset;
  }

  void Camera::update_direction_from_angles() {
    GLfloat cos_pitch = glm::cos(glm::radians(pitch_));
    GLfloat sin_pitch = glm::sin(glm::radians(pitch_));
    GLfloat cos_yaw = glm::cos(glm::radians(yaw_));
    GLfloat sin_yaw = glm::sin(glm::radians(yaw_));

    direction = glm::normalize(glm::vec3(cos_pitch * cos_yaw,
                                         sin_pitch,
                                         cos_pitch * sin_yaw));

  }

  GLfloat Camera::base_speed() const {
    return base_speed_;
  }
}
