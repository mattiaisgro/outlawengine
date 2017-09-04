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

			// TO-DO
			// inline void perspective() {
			// projection = uroboro::perspective();
			// }

			// inline void ortho() {
			// projection = uroboro::ortho();
			// }

	};

}

#endif
