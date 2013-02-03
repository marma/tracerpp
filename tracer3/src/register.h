#ifndef __REGISTER_H__
#define __REGISTER_H__

#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/mesh.h"
#include "shapes/aabox.h"
#include "shapes/csg.h"
#include "shaders/phongshader.h"
#include "shaders/noiseshader.h"
#include "shaders/mirrorshader.h"
#include "shaders/checkershader.h"
#include "shaders/glassshader.h"
#include "shaders/chromerust.h"
#include "shaders/clearshader.h"
#include "shaders/normalshader.h"
#include "shaders/flatshader.h"
#include "lightsources/simplelightsource.h"
#include "lightsources/simplearealightsource.h"
#include "lightsources/stringlight.h"
#include "lightsources/skylightsource.h"
#include "lightsources/skysphere.h"
#include "lightsource_factory.h"
#include "shape_factory.h"
#include "shader_factory.h"

namespace tracer {
	void registerCommons() {
		tracer::lightsource_factory::registerLightsource(new SimpleLightsource());
		tracer::lightsource_factory::registerLightsource(new SimpleAreaLightsource());
		tracer::lightsource_factory::registerLightsource(new StringLightsource());
                tracer::lightsource_factory::registerLightsource(new SkyLightsource());
                tracer::lightsource_factory::registerLightsource(new Skysphere());

		tracer::shape_factory::registerShape(new Plane());
		tracer::shape_factory::registerShape(new Sphere());
		tracer::shape_factory::registerShape(new Mesh());
		tracer::shape_factory::registerShape(new AABox());
		tracer::shape_factory::registerShape(new CSG());

		tracer::shader_factory::registerShader(new ClearShader());
		tracer::shader_factory::registerShader(new PhongShader());
		tracer::shader_factory::registerShader(new NoiseShader());
		tracer::shader_factory::registerShader(new MirrorShader());
		tracer::shader_factory::registerShader(new CheckerShader());
		tracer::shader_factory::registerShader(new GlassShader());
		tracer::shader_factory::registerShader(new ChromeRust());
		tracer::shader_factory::registerShader(new NormalShader());
		tracer::shader_factory::registerShader(new FlatShader());

		//		tracer::volumeshader_factory::registerVolumeShader(new Fog());
		//                tracer::volumeshader_factory::registerVolumeShader(new SimpleVolumeShader());
	}
}

#endif
