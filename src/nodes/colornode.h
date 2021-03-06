//
//  colornode.h
//  C-Ray
//
//  Created by Valtteri Koskivuori on 30/11/2020.
//  Copyright © 2020 Valtteri Koskivuori. All rights reserved.
//

#pragma once

#include "nodebase.h"

enum textureType {
	Diffuse,
	Normal,
	Specular
};

struct colorNode {
	struct nodeBase base;
	struct color (*eval)(const struct colorNode *node, const struct hitRecord *record);
};

#include "textures/checker.h"
#include "textures/constant.h"
#include "textures/image.h"
#include "textures/gradient.h"
#include "converter/grayscale.h"
#include "converter/blackbody.h"
#include "converter/combine.h"
#include "converter/combinergb.h"

const struct colorNode *unknownTextureNode(const struct world *world);
