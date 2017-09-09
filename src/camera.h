#ifndef OUTLAW_CAMERA_H
#define OUTLAW_CAMERA_H
#include "types.h"

namespace outlaw {

	class Camera {
		public:
			mat4 view;
			mat4 projection;

			inline Camera(mat4 view = mat4(), mat4 projection = mat4())
							: view(view), projection(projection) {}

			inline void lookAt(vec3 position, vec3 target, vec3 up) {
				// view = uroboro::lookAt(position, target, up);
			}

			inline void perspective(float fov, float aspect, float near = 0.1f, float far = 100.f) {

				projection = uroboro::perspective(fov, aspect, near, far);
			}

			inline void ortho(float left, float right, float bottom, float top, float near, float far) {

				projection = uroboro::ortho(left, right, bottom, top, near, far);
			}

	};

}

#endif
