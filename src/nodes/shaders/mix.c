//
//  mix.c
//  C-Ray
//
//  Created by Valtteri Koskivuori on 30/11/2020.
//  Copyright © 2020 Valtteri Koskivuori. All rights reserved.
//

#include "../../includes.h"
#include "../../datatypes/color.h"
#include "../../renderer/samplers/sampler.h"
#include "../../datatypes/vector.h"
#include "../../datatypes/material.h"
#include "../../utils/hashtable.h"
#include "../../datatypes/scene.h"
#include "../bsdfnode.h"

#include "mix.h"

struct mixBsdf {
	struct bsdfNode bsdf;
	const struct bsdfNode *A;
	const struct bsdfNode *B;
	const struct valueNode *factor;
};

static struct bsdfSample sample(const struct bsdfNode *bsdf, sampler *sampler, const struct hitRecord *record) {
	struct mixBsdf *mixBsdf = (struct mixBsdf*)bsdf;
	//TODO: Do we need grayscale()?
	const float lerp = mixBsdf->factor->eval(mixBsdf->factor, record);
	if (getDimension(sampler) < lerp) {
		return mixBsdf->A->sample(mixBsdf->A, sampler, record);
	} else {
		return mixBsdf->B->sample(mixBsdf->B, sampler, record);
	}
}

static bool compareMix(const void *A, const void *B) {
	struct mixBsdf *this = (struct mixBsdf *)A;
	struct mixBsdf *other = (struct mixBsdf *)B;
	return this->A == other->A && this->B == other->B && this->factor == other->factor;
}

static uint32_t hashMix(const void *p) {
	const struct mixBsdf *this = p;
	uint32_t h = hashInit();
	h = hashBytes(h, &this->A, sizeof(this->A));
	h = hashBytes(h, &this->B, sizeof(this->B));
	h = hashBytes(h, &this->factor, sizeof(this->factor));
	return h;
}

const struct bsdfNode *newMix(const struct world *world, const struct bsdfNode *A, const struct bsdfNode *B, const struct valueNode *factor) {
	if (A == B) {
		logr(debug, "A == B, pruning mix node.\n");
		return A;
	}
	HASH_CONS(world->nodeTable, &world->nodePool, hashMix, struct mixBsdf, {
		.A = A ? A : newDiffuse(world, newConstantTexture(world, blackColor)),
		.B = B ? B : newDiffuse(world, newConstantTexture(world, blackColor)),
		.factor = factor ? factor : newConstantValue(world, 0.5f),
		.bsdf = {
			.sample = sample,
			.base = { .compare = compareMix }
		}
	});
}
