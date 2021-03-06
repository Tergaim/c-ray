//
//  pathtrace.c
//  C-ray
//
//  Created by Valtteri Koskivuori on 27/04/2017.
//  Copyright © 2015-2020 Valtteri Koskivuori. All rights reserved.
//

#include "../includes.h"
#include "pathtrace.h"

#include "../datatypes/scene.h"
#include "../datatypes/camera.h"
#include "../accelerators/bvh.h"
#include "../datatypes/image/texture.h"
#include "../datatypes/vertexbuffer.h"
#include "../datatypes/sphere.h"
#include "../datatypes/poly.h"
#include "../datatypes/mesh.h"
#include "samplers/sampler.h"
#include "sky.h"
#include "../datatypes/transforms.h"
#include "../datatypes/instance.h"

static struct hitRecord getClosestIsect(struct lightRay *incidentRay, const struct world *scene);

struct color debugNormals(const struct lightRay *incidentRay, const struct world *scene, int maxDepth, sampler *sampler) {
	(void)maxDepth;
	(void)sampler;
	struct lightRay currentRay = *incidentRay;
	struct hitRecord isect = getClosestIsect(&currentRay, scene);
	if (isect.instIndex < 0)
		return scene->background->sample(scene->background, sampler, &isect).color;//getBackground(&currentRay, scene);
	struct vector normal =  isect.surfaceNormal;
	return (struct color){fabs(normal.x), fabs(normal.y), fabs(normal.z), 1.0f};
}

struct color pathTrace(const struct lightRay *incidentRay, const struct world *scene, int maxDepth, sampler *sampler) {
#ifdef DBG_NORMALS
	return debugNormals(incidentRay, scene, maxDepth, sampler);
#endif
	struct color weight = whiteColor; // Current path weight
	struct color finalColor = blackColor; // Final path contribution
	struct lightRay currentRay = *incidentRay;
	
	for (int depth = 0; depth < maxDepth; ++depth) {
		const struct hitRecord isect = getClosestIsect(&currentRay, scene);
		if (isect.instIndex < 0) {
			finalColor = addColors(finalColor, multiplyColors(weight, scene->background->sample(scene->background, sampler, &isect).color));
			break;
		}
		
		finalColor = addColors(finalColor, multiplyColors(weight, isect.material.emission));
		
		struct bsdfSample sample = isect.material.bsdf->sample(isect.material.bsdf, sampler, &isect);
		currentRay.start = isect.hitPoint;
		currentRay.direction = sample.out;
		struct color attenuation = sample.color;
		
		float probability = 1.0f;
		if (depth >= 4) {
			probability = max(attenuation.red, max(attenuation.green, attenuation.blue));
			if (getDimension(sampler) > probability)
				break;
		}
		
		weight = colorCoef(1.0f / probability, multiplyColors(attenuation, weight));
	}
	return finalColor;
}

/**
 Calculate the closest intersection point, and other relevant information based on a given lightRay and scene
 See the intersection struct for documentation of what this function calculates.

 @param incidentRay Given light ray (set up in renderThread())
 @param scene  Given scene to cast that ray into
 @return intersection struct with the appropriate values set
 */
static struct hitRecord getClosestIsect(struct lightRay *incidentRay, const struct world *scene) {
	struct hitRecord isect;
	isect.instIndex = -1;
	isect.distance = FLT_MAX;
	isect.incident = *incidentRay;
	isect.polygon = NULL;
	
	traverseTopLevelBvh(scene->instances, scene->topLevel, incidentRay, &isect);
	
	return isect;
}
