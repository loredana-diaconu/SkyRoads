#pragma once

#include <include/glm.h>
#include <include/math.h>

namespace Laborator {
	class Camera {
		public:
			Camera() {
				position = glm::vec3(0, 2, 5);
				forward = glm::vec3(0, 0, -1);
				up		= glm::vec3(0, 1, 0);
				right	= glm::vec3(1, 0, 0);
				distanceToTarget = 2;
			}

			Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up) {
				Set(position, center, up);
			}

			~Camera() { }

			void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up) {
				this->position = position;
				forward = glm::normalize(center-position);
				right	= glm::cross(forward, up);
				this->up = glm::cross(right,forward);
			}

			glm::mat4 GetViewMatrix() {
				return glm::lookAt(position, position + forward, up);
			}

			glm::vec3 GetTargetPosition() {
				return position + forward * distanceToTarget;
			}

		public:
			float distanceToTarget;
			glm::vec3 position;
			glm::vec3 forward;
			glm::vec3 right;
			glm::vec3 up;
		};
}