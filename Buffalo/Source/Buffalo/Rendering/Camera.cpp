#include <Buffalo/Rendering/Camera.hpp>

namespace Buffalo {
	namespace Rendering {
		Camera::Camera(const glm::mat4& view, const glm::mat4& projection)
			: location(0.f, 0.f, 1.f)
			, orientation(glm::quatLookAt(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f)))
			, viewMatrix(view)
			, projectionMatrix(projection)
			, viewProjectionMatrix(CalculateViewProjectionMatrix()) {

		}

		Camera::~Camera() = default;

		glm::mat4 Camera::CalculateViewProjectionMatrix() {
			return projectionMatrix * viewMatrix;
		}

		void Camera::SetLocation(const glm::vec3& newLocation) {
			location = newLocation;

			viewMatrix = CalculateViewMatrix();
			viewProjectionMatrix = CalculateViewProjectionMatrix();
		}

		void Camera::ApplyRotation(const glm::quat& rotation) {
			orientation = orientation * rotation;

			viewMatrix = CalculateViewMatrix();
			viewProjectionMatrix = CalculateViewProjectionMatrix();
		}
	}
}